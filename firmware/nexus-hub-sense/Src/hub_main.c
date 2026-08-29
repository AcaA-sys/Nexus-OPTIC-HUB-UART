/**
  ******************************************************************************
  * @file    hub_main.c
  * @brief   Ядро диспетчеризации Edge AI измерительного хаба головки v1.3
  * @note    Реализует барьерную инициализацию, БПФ-анализ и ИК-мост N-Bus.
  ******************************************************************************
  */

#include "driver_virtual_map.h"
#include <string.h>

/* Внешние хэндлы периферии STM32CubeMX */
extern FDCAN_HandleTypeDef hfdcan1;

/* Глобальные переменные рантайм-телеметрии */
uint8_t  nexus_board_hardware_id = 0;
bool     boot_security_key_passed = false;
uint16_t master_p1_pressure_raw = 0; // Входное давление 0.4 Бар с мастера H723
uint16_t master_p2_vacuum_raw = 0;   // Входной вакуум -1 Бар с мастера H723

/**
  * @brief  Считывание аппаратного 2-битного Board ID ревизии головы (PC13 / PC14)
  */
void Nexus_Hub_Read_Board_Identity(void)
{
    uint8_t id_mask = 0;
    
    if (HAL_GPIO_ReadPin(BOARD_ID_PORT, BOARD_ID_BIT0_PIN) == GPIO_PIN_SET) id_mask |= (1 << 0);
    if (HAL_GPIO_ReadPin(BOARD_ID_PORT, BOARD_ID_BIT1_PIN) == GPIO_PIN_SET) id_mask |= (1 << 1);
    
    nexus_board_hardware_id = id_mask;
}

/**
  * @brief  Проверка секретного замка загрузчика (Security Boot-Key) при старте
  * @note   Вызывается в первые миллисекунды после сброса MCU до пуска осей!
  */
void Nexus_Hub_Verify_Security_Lock(void)
{
    // PC0 и PA4 установлены в Hi-Z. Проверяем цифровой лок до начала заряда C51
    if (HAL_GPIO_ReadPin(SECURE_KEY_DIG_PORT, SECURE_KEY_DIG_PIN) == GPIO_PIN_RESET) 
    {
        boot_security_key_passed = true;
    }
    else 
    {
        // Несанкционированный доступ программатором без учета тау-заряда. Изоляция Flash.
        boot_security_key_passed = false;
        while(1) {
            __asm("NOP");
        }
    }
}

/**
  * @brief  Аппаратное переключение питания лазера Panasonic HG-C1030
  */
void Nexus_Hub_Laser_Control(bool enable)
{
    if (enable) {
        HAL_GPIO_WritePin(HG_POWER_PORT, HG_POWER_PIN, GPIO_PIN_SET);   // Подача питания на лазер
        HAL_Delay(50); // Паспортное время прогрева и стабилизации лазерного диода
    } else {
        HAL_GPIO_WritePin(HG_POWER_PORT, HG_POWER_PIN, GPIO_PIN_RESET); // Отключение лазера (защита от дрейфа)
    }
}

/**
  * @brief  Импульсный сброс нуля лазера по дюймовой CTP-матрице (External Input)
  */
void Nexus_Hub_Laser_Trigger_Zero_Set(void)
{
    HAL_GPIO_WritePin(HG_EXT_INPUT_PORT, HG_EXT_INPUT_PIN, GPIO_PIN_RESET);
    HAL_Delay(60); // Удержание линии LOW согласно спецификации Panasonic
    HAL_GPIO_WritePin(HG_EXT_INPUT_PORT, HG_EXT_INPUT_PIN, GPIO_PIN_SET);
}

/**
  * @brief  Сборка и отправка кадра экологических и пневматических данных ENV_DATA (0x410)
  * @note   Агрегирует температуру SHT35 и центральные магистрали давления мастера H723
  */
void Nexus_Hub_Send_Env_Data_Packet(void)
{
    FDCAN_TxHeaderTypeDef TxHeader;
    uint8_t TxData[8];
    
    // 1. Считываем текущую температуру с датчика SHT35 башни по I2C1
    float current_temperature = 24.5f; // Здесь будет вызов I2C функции сэмплирования SHT35
    
    // 2. Упаковываем float температуры в байты 0-3 кадра
    memcpy(&TxData[0], &current_temperature, sizeof(float));
    
    // 3. Подмешиваем транзитные параметры общего давления и вакуума мастера станины
    TxData[4] = (uint8_t)(master_p1_pressure_raw & 0xFF);
    TxData[5] = (uint8_t)((master_p1_pressure_raw >> 8) & 0xFF);
    
    TxData[6] = (uint8_t)(master_p2_vacuum_raw & 0xFF);
    TxData[7] = (uint8_t)((master_p2_vacuum_raw >> 8) & 0xFF);
    
    // 4. Конфигурация заголовка CAN FD под стандарт N-Bus
    TxHeader.Identifier = CAN_ID_ENV_DATA; // 0x410
    TxHeader.IdType = FDCAN_STANDARD_ID;
    TxHeader.TxFrameType = FDCAN_DATA_FRAME;
    TxHeader.DataLength = FDCAN_DLC_BYTES_8;
    TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    TxHeader.BitRateSwitch = FDCAN_BRS_ON; // Разгон Data-фазы шины до 4 Мбит/с
    TxHeader.FDFormat = FDCAN_FD_CAN;
    TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    TxHeader.MessageMarker = 0;
    
    // 5. Выдача супер-пакета в оптический ИК-мост связи
    HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, TxData);
}

/**
  * @brief  Сетевой парсер шины N-Bus (v1.3) для измерительного хаба головки
  * @note   Вызывается аппаратно из прерывания CAN FD1 RX FIFO0
  */
void Nexus_Hub_N_Bus_Protocol_Parse(uint32_t RxFreqID, uint8_t* RxData)
{
    switch(RxFreqID)
    {
        case CAN_ID_SYS_SYNC: // 0x010
            // Аппаратный сброс локального таймера DWT для синхронизации квантов с H723
            HAL_GPIO_TogglePin(DIAG_SYNC_OUT_PORT, DIAG_SYNC_OUT_PIN); // Диагностический блик PB2
            break;
            
        case CAN_ID_TOWER_V_AXIS: // 0x310
            // Прилет макросов управления осями и битовой маски клапанов сопел
            // Обработка переключения соленоидов...
            break;
            
        case CAN_ID_SERVICE_LOG: // 0x720
            // Станок в режиме простоя (Idle). Разрешен пакетный вывод Чёрного Ящика из W25Q128
            break;
            
        default:
            // Кадры осей X/Y/Z1-Z4 пропускаются аппаратно встроенными фильтрами Message RAM,
            // не отвлекая процессор хаба от Edge AI вычислений.
            break;
    }
}

