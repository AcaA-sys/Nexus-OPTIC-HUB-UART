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

================================================================================
          ПОЛНАЯ МАТРИЦА АЛЛОКАЦИИ ИДЕНТИФИКАТОРОВ ШИНЫ N-BUS (СТАНДАРТ v1.3)
================================================================================
Приоритет в арбитраже шины жестко определяется значением ID: ниже код = выше приоритет.

+----------+--------------------+-------------+--------------------------------+

| ID (Hex) | Имя кадра (Name)   | Источник    | Описание и структура данных    |
+----------+--------------------+-------------+--------------------------------+

| 0x010    | SYS_SYNC           | Master      | Глобальный синхроимпульс такта |
| 0x050    | RT_CORRECTION      | Master      | Офсет активной компенсации X/Y |
| 0x120    | ACCEL_DATA         | Hub-Sense   | 20-бит поток вибрации (ISM330) |
|          |                    |             |                                |
| 0x201    | DRIVE_X_CMD        | Master      | Уставка координаты/скорости X  |
| 0x202    | DRIVE_Y_CMD        | Master      | Уставка координаты/скорости Y  |
| 0x203    | DRIVE_Z1_CMD       | Master      | Целевая высота сопла Z1        |
| 0x204    | DRIVE_Z2_CMD       | Master      | Целевая высота сопла Z2        |
| 0x205    | DRIVE_Z3_CMD       | Master      | Целевая высота сопла Z3        |
| 0x206    | DRIVE_Z4_CMD       | Master      | Целевая высота сопла Z4        |
|          |                    |             |                                |
| 0x281    | DRIVE_X_TELEMETRY  | Drive-X     | actual_pos, following_error,   |
|          |                    |             | биты флагов IN_POSITION/ERROR  |
| 0x282    | DRIVE_Y_TELEMETRY  | Drive-Y     | actual_pos, following_error,   |
|          |                    |             | биты флагов IN_POSITION/ERROR  |
| 0x283    | DRIVE_Z1_TELEMETRY | Drive-Z1    | Код энкодера A/B/Z и статус Z1 |
| 0x284    | DRIVE_Z2_TELEMETRY | Drive-Z2    | Код энкодера A/B/Z и статус Z2 |
| 0x285    | DRIVE_Z3_TELEMETRY | Drive-Z3    | Код энкодера A/B/Z и статус Z3 |
| 0x286    | DRIVE_Z4_TELEMETRY | Drive-Z4    | Код энкодера A/B/Z и статус Z4 |
|          |                    |             |                                |
| 0x310    | TOWER_V_AXIS       | Master      | Групповой макрос осей R1/R2    |
| 0x311    | TOWER_STATUS       | Hub-Sense   | Локальный аналоговый вакуум    |
|          |                    |             | четырех присосок сопел Vac_S1-4|
|          |                    |             |                                |
| 0x410    | ENV_DATA           | Hub-Sense   | Температура SHT35 башни головы |
|          |                    | (+Master)   | + Давление P1 (0.4 Бар)        |
|          |                    |             | + Вакуум P2 (-1 Бар) с H723    |
|          |                    |             |                                |
| 0x720    | SERVICE_LOG        | Hub-Sense   | Пакетный Burst-вывод Чёрного  |
|          |                    |             | Ящика (W25Q128) на холостом ходу|
+----------+--------------------+-------------+--------------------------------+

--------------------------------------------------------------------------------
БИТОВАЯ СТРУКТУРА НОВЫХ И МОДЕРНИЗИРОВАННЫХ КАДРОВ ШИНЫ (v1.3):
--------------------------------------------------------------------------------

1. Кадр TOWER_STATUS (ID: 0x311, DLC = 8)
   Передается измерительным хабом головы на станину. Содержит оцифровку аналоговых 
   датчиков вакуума сопел для ИИ-контроля качества захвата чипа:
   - Bytes 0-1: Vac_S1 (uint16_t, 12-бит АЦП, сопло 1)
   - Bytes 2-3: Vac_S2 (uint16_t, 12-бит АЦП, сопло 2)
   - Bytes 4-5: Vac_S3 (uint16_t, 12-бит АЦП, сопло 3)
   - Bytes 6-7: Vac_S4 (uint16_t, 12-бит АЦП, сопло 4)

2. Кадр ENV_DATA (ID: 0x410, DLC = 8)
   Агрегирует тепловые параметры башни и опорные давления центральных магистралей 
   станины для динамической адаптации прижима под массу/размер детали:
   - Bytes 0-3: Head_Temperature (float, данные датчика SHT35 в °C)
   - Bytes 4-5: Master_Pres_P1   (uint16_t, 12-бит АЦП, входное давление 0.4 Бар)
   - Bytes 6-7: Master_Vac_P2    (uint16_t, 12-бит АЦП, входной вакуум -1 Бар)

3. Кадры DRIVE_Z1_TELEMETRY .. DRIVE_Z4_TELEMETRY (ID: 0x283 - 0x286, DLC = 6)
   Передают физическое положение вертикального хода сопел Z:
   - Bytes 0-1: Z_Enc_AB (uint16_t, сырой код квадрантов счетчика высоты)
   - Byte 2:    Z_Enc_Z  (uint8_t, флаг прохождения индексной метки нуля, 0 или 1)
   - Byte 3:    Z_Status (uint8_t, битовая маска концевиков допуска)
   - Bytes 4-5: Reserved (uint16_t, системный резерв выравнивания)
================================================================================


/* ========================================================================== */
/* 4. ДОБАВЛЕННЫЕ АНАЛОГОВЫЕ МАГИСТРАЛИ ДАВЛЕНИЯ МАСТЕР-ПЛАТЫ                 */
/* ========================================================================== */
// Оцифровка 12-бит АЦП мастера, передача транзитом в лог ENV_DATA (0x410)
#define MASTER_PRES_P1_PIN         GPIOA_PIN_2 // Вход избыточного давления 0.4 Бар (Мастер H723)
#define MASTER_VAC_P2_PIN          GPIOA_PIN_3 // Вход центрального вакуума -1 Бар (Мастер H723)
#endif
/* ========================================================================== */
/* 5. ДОМЕН ТАКТИРОВАНИЯ И СЕТЕВОГО ОБМЕНА N-BUS (CAN FD1 / ИК-МОСТ)          */
/* ========================================================================== */
#define HSE_CRYSTAL_FREQ_MHZ       24         // Внешний прецизионный кварц HSE
#define FDCAN_PERIPH_CLK_MHZ       80         // Частота шины FDCAN после PLL (Выравнивание N-Bus)

#define IR_FDCAN_RX_PORT           GPIOA
#define IR_FDCAN_RX_PIN            GPIO_PIN_11 // Ножка 45: FDCAN1_RX (AF9)
#define IR_FDCAN_TX_PORT           GPIOA
#define IR_FDCAN_TX_PIN            GPIO_PIN_12 // Ножка 46: FDCAN1_TX (AF9)

/* ========================================================================== */
/* 6. ДОМЕН ИЗМЕРИТЕЛЬНЫХ ДАТЧИКОВ БАШНИ ГОЛОВКИ                              */
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
/* 7. КОНТУР БЕЗОПАСНОСТИ, АВТОРСКОЙ СЕКРЕТКИ И ИДЕНТИФИКАЦИИ                 */
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
/* 8. РЕЗЕРВНЫЕ ИНТЕРФЕЙСЫ И ЛИНЕЙКИ РАСШИРЕНИЯ                               */
/* ========================================================================== */
// Локальный свободный аппаратный резерв (Ножки 30, 31, 32 LQFP64)
#define EXPANSION_RESERVE_PORT     GPIOB
#define EXPANSION_PB11_PIN         GPIO_PIN_11
#define EXPANSION_PB12_PIN         GPIO_PIN_12
#define EXPANSION_PB13_PIN         GPIO_PIN_13

// Свободный аналогово-цифровой резерв общего назначения (Test Points)
#define SPARE_PA0_PIN              GPIO_PIN_0  // Ножка 12: PA0


#endif /* DRIVER_VIRTUAL_MAP_H */
