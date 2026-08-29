/**
  ******************************************************************************
  * @file    driver_virtual_map.h
  * @brief   Аппаратная карта CAN ID и пинов под стандарт N-Bus (v1.3)
  * @part    STM32G474RET6 (LQFP64) / Репозиторий: Nexus-OPTIC-HUB-UART
  * @note    Защищено сквозными контурами безопасности и термокомпенсации.
  ******************************************************************************
  */

#ifndef DRIVER_VIRTUAL_MAP_H
#define DRIVER_VIRTUAL_MAP_H

#include "main.h"

/* ========================================================================== */
/* 1. ПОЛНАЯ МАТРИЦА АЛЛОКАЦИИ ИДЕНТИФИКАТОРОВ ШИНЫ N-BUS (СТАНДАРТ v1.3)     */
/* ========================================================================== */
#define CAN_ID_SYS_SYNC            0x010  // Глобальный тактовый синхроимпульс (Master)
#define CAN_ID_RT_CORRECTION       0x050  // Активная наносекундная коррекция траектории X/Y
#define CAN_ID_ACCEL_DATA          0x120  // Высокопрецизионный 20-бит поток вибрации головки

#define CAN_ID_DRIVE_X_CMD         0x201  // Уставка координаты/скорости оси X
#define CAN_ID_DRIVE_Y_CMD         0x202  // Уставка координаты/скорости оси Y
#define CAN_ID_DRIVE_Z1_CMD        0x203  // Целевая высота вертикального сопла Z1
#define CAN_ID_DRIVE_Z2_CMD        0x204  // Целевая высота вертикального сопла Z2
#define CAN_ID_DRIVE_Z3_CMD        0x205  // Целевая высота вертикального сопла Z3
#define CAN_ID_DRIVE_Z4_CMD        0x206  // Целевая высота вертикального сопла Z4

#define CAN_ID_DRIVE_X_TELEMETRY   0x281  // Телеметрия Closed-Loop оси X
#define CAN_ID_DRIVE_Y_TELEMETRY   0x282  // Телеметрия Closed-Loop оси Y
#define CAN_ID_DRIVE_Z1_TELEMETRY  0x283  // Код инкрементального энкодера A/B/Z оси Z1
#define CAN_ID_DRIVE_Z2_TELEMETRY  0x284  // Код инкрементального энкодера A/B/Z оси Z2
#define CAN_ID_DRIVE_Z3_TELEMETRY  0x285  // Код инкрементального энкодера A/B/Z оси Z3
#define CAN_ID_DRIVE_Z4_TELEMETRY  0x286  // Код инкрементального энкодера A/B/Z оси Z4

#define CAN_ID_TOWER_V_AXIS        0x310  // Групповой макрос осей вращения R1/R2
#define CAN_ID_TOWER_STATUS        0x311  // Локальный АЦП-вакуум присосок сопел Vac_S1-4
#define CAN_ID_ENV_DATA            0x410  // Температура SHT35 + Опорные давления P1/P2 мастера
#define CAN_ID_SERVICE_LOG         0x720  // Чтение Чёрного Ящика (W25Q128) при простое ЧПУ

/* ========================================================================== */
/* 2. ДОМЕН СЕТЕВОГО ОБМЕНА И ТАКТИРОВАНИЯ (FDCAN1 / ИК-МОСТ)                 */
/* ========================================================================== */
#define HSE_CRYSTAL_FREQ_MHZ       24     // Внешний прецизионный кварц HSE
#define FDCAN_PERIPH_CLK_MHZ       80     // Тактовая частота шины CAN после PLL

#define IR_FDCAN_RX_PORT           GPIOA
#define IR_FDCAN_RX_PIN            GPIO_PIN_11 // Ножка 45: FDCAN1_RX (AF9)
#define IR_FDCAN_TX_PORT           GPIOA
#define IR_FDCAN_TX_PIN            GPIO_PIN_12 // Ножка 46: FDCAN1_TX (AF9)

/* ========================================================================== */
/* 3. ДОМЕН ИЗМЕРИТЕЛЬНЫХ ДАТЧИКОВ ЛЕТАЮЩЕЙ БАШНИ ГОЛОВКИ                     */
/* ========================================================================== */
// Лазерный триангуляционный датчик Panasonic HG-C1030
#define HG_ADC_PORT                GPIOB
#define HG_ADC_PIN                 GPIO_PIN_1  // Ножка 18: ADC3_IN5 (Расстояние сканирования)
#define HG_EXT_INPUT_PORT          GPIOC
#define HG_EXT_INPUT_PIN           GPIO_PIN_6  // Ножка 37: GPIO_Output (Zero Set / Hold)
#define HG_POWER_PORT              GPIOC
#define HG_POWER_PIN               GPIO_PIN_15 // Ножка 3:  GPIO_Output (ON/OFF лазерного диода)

// Промышленный 6-осевой IMU-акселерометр ISM330DHCX (Шина SPI1)
#define IMU_SPI                    SPI1
#define IMU_CS_PORT                GPIOC
#define IMU_CS_PIN                 GPIO_PIN_5  // Ножка 24: GPIO_Output (Chip Select)
#define IMU_INT1_PORT              GPIOB
#define IMU_INT1_PIN               GPIO_PIN_0  // Ножка 26: EXTI Прерывание 1
#define IMU_INT2_PORT              GPIOA
#define IMU_INT2_PIN               GPIO_PIN_1  // Ножка 13: EXTI Прерывание 2 (Добавлено!)

// Датчик освещенности OPT3001 (Шина I2C1)
#define OPT_INT3_PORT              GPIOC
#define OPT_INT3_PIN               GPIO_PIN_7  // Ножка 38: EXTI Прерывание 3

// Акустический дифференциальный MEMS-микрофон IM73A135V01
#define MIC_ADC_PORT               GPIOB
#define MIC_DIFF_P_PIN             GPIO_PIN_14 // Ножка 47: ADC4_IN1 (Дифференциальный +)
#define MIC_DIFF_N_PIN             GPIO_PIN_15 // Ножка 48: ADC4_IN2 (Дифференциальный -)

/* ========================================================================== */
/* 4. КОНТУР БЕЗОПАСНОСТИ, АВТОРСКОЙ СЕКРЕТКИ И ИДЕНТИФИКАЦИИ                 */
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

// Выделенный диагностический выход луча
#define DIAG_SYNC_OUT_PORT         GPIOB
#define DIAG_SYNC_OUT_PIN          GPIO_PIN_2  // Ножка 26: PB2 (Диагностический SYNC)

/* ========================================================================== */
/* 5. РЕЗЕРВНЫЕ ИНТЕРФЕЙСЫ И ЛИНЕЙКИ РАСШИРЕНИЯ                               */
/* ========================================================================== */
// Локальный аппаратный резерв (Ножки 30, 31, 32 LQFP64)
#define EXPANSION_RESERVE_PORT     GPIOB
#define EXPANSION_PB11_PIN         GPIO_PIN_11
#define EXPANSION_PB12_PIN         GPIO_PIN_12
#define EXPANSION_PB13_PIN         GPIO_PIN_13

// Свободный аналогово-цифровой резерв общего назначения (Test Points)
#define SPARE_PA0_PIN              GPIO_PIN_0  // Ножка 12: PA0
#define SPARE_PA1_PIN              GPIO_PIN_1  // Ножка 13: PA1

#endif /* DRIVER_VIRTUAL_MAP_H */

