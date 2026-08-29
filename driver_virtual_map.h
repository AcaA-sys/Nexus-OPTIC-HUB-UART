/**
  ******************************************************************************
  * @file    driver_virtual_map.h
  * @brief   Аппаратная карта пинов и периферии хаба Nexus-Hub-Sense (v2.0)
  * @part    STM32G474RET6 (LQFP64)
  ******************************************************************************
  */

#ifndef DRIVER_VIRTUAL_MAP_H
#define DRIVER_VIRTUAL_MAP_H

#include "main.h"

/* ========================================================================== */
/* 1. ФИКСИРОВАННЫЕ ИДЕНТИФИКАТОРЫ ШИНЫ N-BUS (СТАНДАРТ CAN ID v1.2)          */
/* ========================================================================== */
#define CAN_ID_SYS_SYNC            0x010  // Глобальный тактовый синхроимпульс (Master)
#define CAN_ID_RT_CORRECTION       0x050  // Активная наносекундная коррекция траектории X/Y
#define CAN_ID_ACCEL_DATA          0x120  // Высокопрецизионный 20-бит поток вибрации головки
#define CAN_ID_DRIVE_X_CMD         0x201  // Основная уставка координаты/скорости оси X
#define CAN_ID_DRIVE_Y_CMD         0x202  // Основная уставка координаты/скорости оси Y
#define CAN_ID_TOWER_V_AXIS        0x310  // Команды макросов виртуальных осей башен
#define CAN_ID_TOWER_STATUS        0x311  // Телеметрия вакуума сопел и концевиков Z
#define CAN_ID_ENV_DATA            0x410  // Данные температуры SHT35 для КТР приводов
#define CAN_ID_SERVICE_LOG         0x720  // Чтение Чёрного Ящика (W25Q128) во время простоя

/* ========================================================================== */
/* 2. ДОБАВЛЕННЫЕ АНАЛОГОВЫЕ МАГИСТРАЛИ ДАВЛЕНИЯ МАСТЕР-ПЛАТЫ                 */
/* ========================================================================== */
// Оцифровка 12-бит АЦП мастера, передача транзитом в лог ENV_DATA (0x410)
#define MASTER_PRES_P1_PIN         GPIOA_PIN_2 // Вход избыточного давления 0.4 Бар (Мастер H723)
#define MASTER_VAC_P2_PIN          GPIOA_PIN_3 // Вход центрального вакуума -1 Бар (Мастер H723)
#endif
/* ========================================================================== */
/* 3. ДОМЕН ТАКТИРОВАНИЯ И СЕТЕВОГО ОБМЕНА N-BUS (CAN FD1 / ИК-МОСТ)          */
/* ========================================================================== */
#define HSE_CRYSTAL_FREQ_MHZ       24         // Внешний прецизионный кварц HSE
#define FDCAN_PERIPH_CLK_MHZ       80         // Частота шины FDCAN после PLL (Выравнивание N-Bus)

#define IR_FDCAN_RX_PORT           GPIOA
#define IR_FDCAN_RX_PIN            GPIO_PIN_11 // Ножка 45: FDCAN1_RX (AF9)
#define IR_FDCAN_TX_PORT           GPIOA
#define IR_FDCAN_TX_PIN            GPIO_PIN_12 // Ножка 46: FDCAN1_TX (AF9)

/* ========================================================================== */
/* 4. ДОМЕН ИЗМЕРИТЕЛЬНЫХ ДАТЧИКОВ БАШНИ ГОЛОВКИ                              */
/* ========================================================================== */
// Лазерный триангуляционный датчик Panasonic HG-C1030
#define HG_ADC_PORT                GPIOB
#define HG_ADC_PIN                 GPIO_PIN_1  // Ножка 18: ADC3_IN5 (Аналоговое расстояние)
#define HG_EXT_INPUT_PORT          GLIOC
#define HG_EXT_INPUT_PIN           GPIO_PIN_6  // Ножка 37: GPIO_Output (Zero Set / Hold)
#define HG_POWER_PORT              GPIOC
#define HG_POWER_PIN               GPIO_PIN_15 // Ножка 3:  GPIO_Output (ON/OFF Лазера)

// Промышленный 6-осевой IMU-акселерометр ISM330DHCX (Шина SPI1)
#define IMU_SPI                    SPI1
#define IMU_CS_PORT                GPIOC
#define IMU_CS_PIN                 GPIO_PIN_5  // Ножка 24: GPIO_Output (Chip Select)
#define IMU_INT1_PORT              GPIOB
#define IMU_INT1_PIN               GPIO_PIN_0  // Ножка 26: EXTI Прерывание 1
#define IMU_INT2_PORT              GPIOA
#define IMU_INT2_PIN               GPIO_PIN_1  // Ножка 13: EXTI Прерывание 2

// Датчик освещенности OPT3001 и влажности/температуры SHT35 (Шина I2C1)
#define SENSORS_I2C                I2C1
#define OPT_INT3_PORT              GPIOC
#define OPT_INT3_PIN               GPIO_PIN_7  // Ножка 38: EXTI Прерывание 3

// Акустический дифференциальный MEMS-микрофон IM73A135V01
#define MIC_ADC_PORT               GPIOB
#define MIC_DIFF_P_PIN             GPIO_PIN_14 // Ножка 47: ADC4_IN1 (Дифференциальный +)
#define MIC_DIFF_N_PIN             GPIO_PIN_15 // Ножка 48: ADC4_IN2 (Дифференциальный -)

/* ========================================================================== */
/* 5. КОНТУР БЕЗОПАСНОСТИ, АВТОРСКОЙ СЕКРЕТКИ И ИДЕНТИФИКАЦИИ                 */
/* ========================================================================== */
// Аппаратный жесткий Board ID (Постоянная подтяжка 10 кОм к +3.3В Цифра)
#define BOARD_ID_PORT              GPIOC
#define BOARD_ID_BIT0_PIN          GPIO_PIN_13 // Ножка 2:  BOARD_ID_BIT0
#define BOARD_ID_BIT1_PIN          GPIO_PIN_14 // Ножка 3:  BOARD_ID_BIT1

// Аппаратная секретка загрузчика (Security Boot-Key RC-замок Тау = 1 сек)
#define SECURE_KEY_ADC_PORT        GPIOA
#define SECURE_KEY_ADC_PIN         GPIO_PIN_4  // Ножка 20: PA4 (Аналоговый контроль АЦП)
#define SECURE_KEY_DIG_PORT        GPIOC
#define SECURE_KEY_DIG_PIN         GPIO_PIN_0  // Ножка 9:  PC0 (Цифровой опрос при старте)

// Сервисный отладочный выход луча
#define DIAG_SYNC_OUT_PORT         GPIOB
#define DIAG_SYNC_OUT_PIN          GPIO_PIN_2  // Ножка 26: PB2 (Диагностический SYNC)

/* ========================================================================== */
/* 6. РЕЗЕРВНЫЕ ИНТЕРФЕЙСЫ И ЛИНЕЙКИ РАСШИРЕНИЯ                               */
/* ========================================================================== */
// Локальный свободный аппаратный резерв (Ножки 30, 31, 32 LQFP64)
#define EXPANSION_RESERVE_PORT     GPIOB
#define EXPANSION_PB11_PIN         GPIO_PIN_11
#define EXPANSION_PB12_PIN         GPIO_PIN_12
#define EXPANSION_PB13_PIN         GPIO_PIN_13

// Свободный аналогово-цифровой резерв общего назначения (Test Points)
#define SPARE_PA0_PIN              GPIO_PIN_0  // Ножка 12: PA0


#endif /* DRIVER_VIRTUAL_MAP_H */
