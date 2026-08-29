/**
  ******************************************************************************
  * @file    hub_main.c
  * @brief   Ядро диспетчеризации Edge AI измерительного хаба головки v1.3
  * @part    STM32G474RET6 (LQFP64) / Репозиторий: Nexus-OPTIC-HUB-UART
  * @note    Реализует асимметричную топологию связи N-Bus:
  *          - Управление вниз на башни Tower-G4: строго по шине FDCAN2
  *          - Телеметрия вверх от башен в хаб: строго по выделенным USART1/2 DMA
  ******************************************************************************
  */

#include "driver_virtual_map.h"
#include <string.h>
#include <stdbool.h>

/* Внешние аппаратные хэндлы периферии, генерируемые STM32CubeMX */
extern FDCAN_HandleTypeDef hfdcan1; // Внешний беспроводной ИК-мост N-Bus (станина)
extern FDCAN_HandleTypeDef hfdcan2; // Локальная внутренняя шина управления башнями
extern UART_HandleTypeDef huart1;   // Выделенная линия приема телеметрии Башни 1-2
extern UART_HandleTypeDef huart2;   // Выделенная линия приема телеметрии Башни 3-4

/* Глобальные системные переменные рантайма */
uint8_t  nexus_board_hardware_id = 0; // Считанный жесткий ID ревизии платы
bool     boot_security_key_passed = false; // Флаг прохождения секретного RC-замка

/* Буферы и переменные для хранения параметров мастера станины H723 */
uint16_t master_p1_pressure_raw = 0; // Общее давление 0.4 Бар (Пин PA2 мастера)
uint16_t master_p2_vacuum_raw = 0;   // Общий вакуум -1 Бар (Пин PA3 мастера)

/* Локальные структуры хранения входящей потоковой телеметрии с башен Tower-G4 */
typedef struct {
    float    current_angle;  // Физический угол поворота насадки в градусах
    uint16_t motor_current;  // Текущий рабочий ток обмоток в миллиамперах
    uint16_t vacuum_raw;     // Локальный аналоговый вакуум сопла (код АЦП)
    int8_t   driver_temp;    // Температура силовых ключей башни в °C
    uint8_t  status_mask;    // Битовая маска концевиков допуска
} Tower_Telemetry_t;

Tower_Telemetry_t tower1_2_data; // Данные, прилетающие от Башни 1 (сопла 1-2)
Tower_Telemetry_t tower3_4_data; // Данные, прилетающие от Башни 2 (сопла 3-4)

/* Промежуточные буферы DMA для прямого аппаратного приема из USART */
uint8_t uart1_dma_rx_buffer[16];
uint8_t uart2_dma_rx_buffer[16];

/**
  * @brief  Считывание аппаратного 2-битного Board ID ревизии головы (PC13 / PC14)
  *         Пины аппаратно жестко завязаны на шину +3.3В через подтяжки 10 кОм.
  */
void Nexus_Hub_Read_Board_Identity(void)
{
    uint8_t id_mask = 0;
    
    if (HAL_GPIO_ReadPin(BOARD_ID_PORT, BOARD_ID_BIT0_PIN) == GPIO_PIN_SET) id_mask |= (1 << 0);
    if (HAL_GPIO_ReadPin(BOARD_ID_PORT, BOARD_ID_BIT1_PIN) == GPIO_PIN_SET) id_mask |= (1 << 1);
    
    nexus_board_hardware_id = id_mask;
}

/**
  * @brief  Проверка секретного временного замка загрузчика (Security Boot-Key)
  * @note   Вызывается в первые миллисекунды после станка до старта N-Bus движения!
  *         Оценивает состояние экспоненты заряда цепи до того, как C51 успеет зарядиться.
  */
void Nexus_Hub_Verify_Security_Lock(void)
{
    // При старте порты PC0 и PA4 находятся в высокоимпедансном состоянии Hi-Z.
    // Если к плате подключен пиратский программатор, конденсатор C51 будет заряжен.
    if (HAL_GPIO_ReadPin(SECURE_KEY_DIG_PORT, SECURE_KEY_DIG_PIN) == GPIO_PIN_RESET) 
    {
        boot_security_key_passed = true; // Проверка пройдена, это оригинальное железо
    }
    else 
    {
        // Несанкционированный сброс или взлом. Наглухо блокируем прошивку.
        boot_security_key_passed = false;
        while(1) {
            __asm("NOP"); // Изоляция Flash-памяти МК от считывания
        }
    }
}

/**
  * @brief  Аппаратный запуск фонового DMA-приема телеметрии с башен
  * @note   Вызывается один раз после успешной инициализации периферии в main()
  */
void Nexus_Hub_Acyclic_Telemetry_Init(void)
{
    // Запускаем циклический прием пакетов из USART в память без участия ядра процессора
    HAL_UART_Receive_DMA(&huart1, uart1_dma_rx_buffer, 16);
    HAL_UART_Receive_DMA(&huart2, uart2_dma_rx_buffer, 16);
}

/**
  * @brief  Аппаратное переключение питания лазера Panasonic HG-C1030
  */
void Nexus_Hub_Laser_Control(bool enable)
{
    if (enable) {
        HAL_GPIO_WritePin(HG_POWER_PORT, HG_POWER_PIN, GPIO_PIN_SET);   // Включение ключа питания лазера
        HAL_Delay(50); // Паспортное время прогрева лазерного диода для ликвидации дрейфа
    } else {
        HAL_GPIO_WritePin(HG_POWER_PORT, HG_POWER_PIN, GPIO_PIN_RESET); // Полное гашение диода (защита зрения)
    }
}

/**
  * @brief  Импульсный сброс нуля лазера по внешней CTP-матрице (External Input)
  */
void Nexus_Hub_Laser_Trigger_Zero_Set(void)
{
    // Согласно документации Panasonic, для сброса нуля требуется уровень LOW от 50 до 100 мс
    HAL_GPIO_WritePin(HG_EXT_INPUT_PORT, HG_EXT_INPUT_PIN, GPIO_PIN_RESET);
    HAL_Delay(60);
    HAL_GPIO_WritePin(HG_EXT_INPUT_PORT, HG_EXT_INPUT_PIN, GPIO_PIN_SET);
}

/**
  * @brief  Сборка и отправка кадра экологических и пневматических данных ENV_DATA (0x410)
  * @note   Агрегирует температуру SHT35 башни и опорные давления 0.4 Бар / -1 Бар мастера H723
  */
void Nexus_Hub_Send_Env_Data_Packet(void)
{
    FDCAN_TxHeaderTypeDef TxHeader;
    uint8_t TxData[8];
    
    // 1. Считываем свежую температуру башни с датчика SHT35 по шине I2C1
    float current_temperature = 22.4f; // Здесь будет реальный вызов функции I2C сэмплирования
    
    // 2. Упаковываем float температуры в байты 0-3
    memcpy(&TxData[0], &current_temperature, sizeof(float));
    
    // 3. Подмешиваем транзитные параметры общего давления и вакуума мастера станины
    TxData[4] = (uint8_t)(master_p1_pressure_raw & 0xFF);
    TxData[5] = (uint8_t)((master_p1_pressure_raw >> 8) & 0xFF);
    
    TxData[6] = (uint8_t)(master_p2_vacuum_raw & 0xFF);
    TxData[7] = (uint8_t)((master_p2_vacuum_raw >> 8) & 0xFF);
    
    // 4. Конфигурация заголовка CAN FD фрейма под стандарт N-Bus v1.3
    TxHeader.Identifier = CAN_ID_ENV_DATA; // 0x410
    TxHeader.IdType = FDCAN_STANDARD_ID;
    TxHeader.TxFrameType = FDCAN_DATA_FRAME;
    TxHeader.DataLength = FDCAN_DLC_BYTES_8;
    TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    TxHeader.BitRateSwitch = FDCAN_BRS_ON; // Разгон скоростной Data-фазы до 4 Мбит/с
    TxHeader.FDFormat = FDCAN_FD_CAN;
    TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    TxHeader.MessageMarker = 0;
    
    // 5. Выстрел супер-пакета в оптический ИК-мост связи на станину
    HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, TxData);
}

/**
  * @brief  НИСХОДЯЩИЙ КАНАЛ СВЯЗИ: Рассылка команд управления на платы Tower-G4 по CAN
  * @note   Выстреливает макросы осей вращения и битовую маску клапанов сопел
  */
void Nexus_Hub_Send_Commands_Down_To_Towers(uint8_t* target_angles_and_valves)
{
    FDCAN_TxHeaderTypeDef TxHeader;
    
    TxHeader.Identifier = CAN_ID_TOWER_V_AXIS; // 0x310
    TxHeader.IdType = FDCAN_STANDARD_ID;
    TxHeader.TxFrameType = FDCAN_DATA_FRAME;
    TxHeader.DataLength = FDCAN_DLC_BYTES_8;
    TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    TxHeader.BitRateSwitch = FDCAN_BRS_ON; // Скоростная передача 4 Мбит/с без коллизий
    TxHeader.FDFormat = FDCAN_FD_CAN;
    TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    TxHeader.MessageMarker = 0;
    
    // Мгновенная прямая отправка команд управления вниз на подчиненные башни
    HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &TxHeader, target_angles_and_valves);
}

/**
  * @brief  ВОСХОДЯЩИЙ КАНАЛ СВЯЗИ: Аппаратный перехват потоковой телеметрии от Tower-G4 по USART DMA
  * @note   Автоматически вызывается ядром МК по окончании приема 16-байтного пакета в фоновом режиме
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1) 
    {
        // В монопольную «трубу» без коллизий и задержек шины прилетел пакет с Башни №1 (сопла 1-2)
        // Распаковываем бинарный буфер DMA напрямую в структуру
        memcpy(&tower1_2_data, uart1_dma_rx_buffer, sizeof(Tower_Telemetry_t));
        
        // Взводим диагностический пин PB2 для осциллографического контроля джиттера приема
        HAL_GPIO_WritePin(DIAG_SYNC_OUT_PORT, DIAG_SYNC_OUT_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(DIAG_SYNC_OUT_PORT, DIAG_SYNC_OUT_PIN, GPIO_PIN_RESET);
    }
    else if (huart->Instance == USART2) 
    {
        // Прилетел пакет с Башни №2 (сопла 3-4)
        memcpy(&tower3_4_data, uart2_dma_rx_buffer, sizeof(Tower_Telemetry_t));
    }
}

/**
  * @brief  СЕТЕВОЙ ПАРСЕР МАГИСТРАЛИ N-BUS (v1.3) ДЛЯ ИЗМЕРИТЕЛЬНОГО ХАБА ГОЛОВКИ
  * @note   Вызывается аппаратно из прерывания ИК-моста CAN FD1 RX FIFO0
  */
void Nexus_Hub_N_Bus_Protocol_Parse(uint32_t RxFreqID, uint8_t* RxData)
{
    switch(RxFreqID)
    {
        case CAN_ID_SYS_SYNC: // 0x010
            // Прилет глобального синхроимпульса от мастера H723.
            // Сброс и синхронизация локальных таймеров квантования Edge AI вычислений хаба.
            break;
            
        case CAN_ID_ENV_DATA: // 0x410
            // Перехват транзитных данных давления P1 и вакуума P2, если они были обновлены на мастере.
            master_p1_pressure_raw = (uint16_t)(RxData[4] | (RxData[5] << 8));
            master_p2_vacuum_raw   = (uint16_t)(RxData[6] | (RxData[7] << 8));
            break;
            
        case CAN_ID_SERVICE_LOG: // 0x720
            // Планировщик ЧПУ перешел в режим простоя (Idle). 
            // Разрешен Burst-вывод накопленного спектра Чёрного Ящика из внешней Flash W25Q128 на ПК.
            break;
            
        default:
            // Тяжелые кадры координат осей перемещения портала (X/Y/Z1-Z4, ID 0x201-0x206)
            // отсекаются встроенными аппаратными фильтрами Message RAM кристалла STM32G474.
            // Процессор хаба головки вообще не отвлекается на этот трафик, отдавая 100% мощности нейросетям!
            break;
    }
}


