
/**
  ******************************************************************************
  * @file    driver_virtual_map.h
  * @brief   Аппаратная карта CAN ID, пинов и пневмо-контуров стандарта N-Bus (v1.3)
  * @part    STM32G474RET6 (LQFP64) / Репозиторий: Nexus-OPTIC-HUB-UART
  * @note    Синхронизировано со спецификацией датчиков XGZP6847 мастера H723.
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
#define CAN_ID_DRIVE_Z1_CMD        0x203  // Целевая высота сопла Z1 (Поддержка до Z8)
#define CAN_ID_DRIVE_Z2_CMD        0x204  // Целевая высота сопла Z2
#define CAN_ID_DRIVE_Z3_CMD        0x205  // Целевая высота сопла Z3
#define CAN_ID_DRIVE_Z4_CMD        0x206  // Целевая высота сопла Z4

#define CAN_ID_DRIVE_X_TELEMETRY   0x281  // Телеметрия Closed-Loop оси X
#define CAN_ID_DRIVE_Y_TELEMETRY   0x282  // Телеметрия Closed-Loop оси Y
#define CAN_ID_DRIVE_Z1_TELEMETRY  0x283  // Код инкрементального энкодера осей Z1-Z4
#define CAN_ID_DRIVE_Z2_TELEMETRY  0x284  
#define CAN_ID_DRIVE_Z3_TELEMETRY  0x285  
#define CAN_ID_DRIVE_Z4_TELEMETRY  0x286  

#define CAN_ID_TOWER_V_AXIS        0x310  // Нисходящий групповой макрос осей R1/R2 (CAN FD2)
#define CAN_ID_TOWER_STATUS        0x311  // Локальный АЦП-вакуум присосок сопел (до 8 штук)
#define CAN_ID_ENV_DATA            0x410  // Температура SHT35 + Опорные давления P1/P2 с мастера
#define CAN_ID_SERVICE_LOG         0x720  // Выгрузка Чёрного Ящика (W25Q128) при простое ЧПУ

/* ========================================================================== */
/* 2. ДОМЕНЫ СЕТЕВОГО ОБМЕНА И РАЗДЕЛЬНЫХ СКОРОСТЕЙ (N-BUS ТАЙМИНГИ)          */
/* ========================================================================== */
#define HSE_CRYSTAL_FREQ_MHZ       24     // Внешний прецизионный кварц HSE хаба
#define CAN_PERIPH_CLK_MHZ         80     // Выровненная тактовая частота шин CAN после PLL

// МЕДНЫЙ ДОМЕН СТАНИНЫ (CAN FD1) - БРОНЕБОЙНЫЕ 4 МБИТ/С
#define COPPER_CAN_RX_PIN          GPIO_PIN_11 // PA11 (AF9)
#define COPPER_CAN_TX_PIN          GPIO_PIN_12 // PA12 (AF9)

// ОПТИЧЕСКИЙ ДОМЕН БАШНИ (CAN FD2 / ИК-ЛУЧ) - РАЗГОН ДО 5 МБИТ/С
#define OPTIC_CAN_RX_PIN           GPIO_PIN_8  // PB8  (AF9)
#define OPTIC_CAN_TX_PIN           GPIO_PIN_9  // PB9  (AF9)

/* ========================================================================== */
/* 3. АППАРАТНАЯ МАТРИЦА ДАТЧИКОВ ДАВЛЕНИЯ МАСТЕР-ПЛАТЫ H723 (РЕЗЕРВ ДЛЯ ТЗ)  */
/* ========================================================================== */
#define VACUUM1_PIN                GPIO_PIN_2  // PA2 (ADC1_IN14) / Датчик P1 давления 0.4 Бар
#define VACUUM2_PIN                GPIO_PIN_3  // PA3 (ADC1_IN15) / Датчик P2 вакуума -1 Бар
#define MASTER_ADC_DIVIDER_RATIO   "10k/20k"   // Резистивное согласование уровней 5V -> 3.3V

/* ========================================================================== */
/* 4. ДОМЕН ИЗМЕРИТЕЛЬНЫХ ДАТЧИКОВ ЛЕТАЮЩЕЙ БАШНИ ГОЛОВКИ                     */
/* ========================================================================== */
// Лазер Panasonic HG-C1030
#define HG_ADC_PORT                GPIOB
#define HG_ADC_PIN                 GPIO_PIN_1  // Ножка 18: ADC3_IN5
#define HG_EXT_INPUT_PORT          GPIOC
#define HG_EXT_INPUT_PIN           GPIO_PIN_6  // Ножка 37: GPIO_Output (Zero/Hold)
#define HG_POWER_PORT              GPIOC
#define HG_POWER_PIN               GPIO_PIN_15 // Ножка 3:  GPIO_Output (ON/OFF лазера)

// Акселерометр ISM330DHCX (SPI1)
#define IMU_SPI                    SPI1
#define IMU_CS_PORT                GPIOC
#define IMU_CS_PIN                 GPIO_PIN_5  // Ножка 24: Chip Select
#define IMU_INT1_PORT              GPIOB
#define IMU_INT1_PIN               GPIO_PIN_0  // Ножка 26: EXTI0
#define IMU_INT2_PORT              GPIOA
#define IMU_INT2_PIN               GPIO_PIN_1  // Ножка 13: EXTI1

// Акустический дифференциальный MEMS-микрофон IM73A135V01
#define MIC_ADC_PORT               GPIOB
#define MIC_DIFF_P_PIN             GPIO_PIN_14 // Ножка 47: ADC4_IN1
#define MIC_DIFF_N_PIN             GPIO_PIN_15 // Ножка 48: ADC4_IN2

/* ========================================================================== */
/* 5. КОНТУР БЕЗОПАСНОСТИ, АВТОРСКОЙ СЕКРЕТКИ И ИДЕНТИФИКАЦИИ                 */
/* ========================================================================== */
#define BOARD_ID_PORT              GPIOC
#define BOARD_ID_BIT0_PIN          GPIO_PIN_13 // Ножка 2:  PC13 (Board ID Bit0)
#define BOARD_ID_BIT1_PIN          GPIO_PIN_14 // Ножка 3:  PC14 (Board ID Bit1)

#define SECURE_KEY_ADC_PORT        GPIOA
#define SECURE_KEY_ADC_PIN         GPIO_PIN_4  // Ножка 20: PA4 (RC-Замок контроль)
#define SECURE_KEY_DIG_PORT        GPIOC
#define SECURE_KEY_DIG_PIN         GPIO_PIN_0  // Ножка 9:  PC0 (Цифровой опрос)

#define DIAG_SYNC_OUT_PORT         GPIOB
#define DIAG_SYNC_OUT_PIN          GPIO_PIN_2  // Ножка 26: PB2 (Диагностический Выход луча)

#endif /* DRIVER_VIRTUAL_MAP_H */

