/**
  ******************************************************************************
  * @file    main.h
  * @brief   Промышленный заголовочный файл-заглушка для верификации N-Bus (v1.3)
  * @note    Изолирует компилятор от тяжелых библиотек STM32 HAL в облаке CI
  ******************************************************************************
  */

#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>
#include <stdbool.h>

/* Базовые логические дефайны для эмуляции HAL */
#define GPIO_PIN_RESET  0
#define GPIO_PIN_SET    1
#define HAL_OK          0

/* Заглушки типов данных и структур для прецизионных Edge AI контуров */
typedef int HAL_StatusTypeDef;
typedef int GPIO_TypeDef;

typedef struct {
    uint32_t Instance;
} FDCAN_HandleTypeDef;

typedef struct {
    uint32_t Instance;
} UART_HandleTypeDef;

typedef struct {
    uint32_t Identifier;
    uint32_t IdType;
    uint32_t TxFrameType;
    uint32_t DataLength;
    uint32_t ErrorStateIndicator;
    uint32_t BitRateSwitch;
    uint32_t FDFormat;
    uint32_t TxEventFifoControl;
    uint32_t MessageMarker;
} FDCAN_TxHeaderTypeDef;

/* Эмуляция констант CubeMX */
#define GPIO_PIN_0   ((uint16_t)0x0001)
#define GPIO_PIN_1   ((uint16_t)0x0002)
#define GPIO_PIN_2   ((uint16_t)0x0004)
#define GPIO_PIN_3   ((uint16_t)0x0008)
#define GPIO_PIN_4   ((uint16_t)0x0010)
#define GPIO_PIN_5   ((uint16_t)0x0020)
#define GPIO_PIN_6   ((uint16_t)0x0040)
#define GPIO_PIN_7   ((uint16_t)0x0080)
#define GPIO_PIN_8   ((uint16_t)0x0100)
#define GPIO_PIN_9   ((uint16_t)0x0200)
#define GPIO_PIN_10  ((uint16_t)0x0400)
#define GPIO_PIN_11  ((uint16_t)0x0800)
#define GPIO_PIN_12  ((uint16_t)0x1000)
#define GPIO_PIN_13  ((uint16_t)0x2000)
#define GPIO_PIN_14  ((uint16_t)0x4000)
#define GPIO_PIN_15  ((uint16_t)0x8000)

#define FDCAN_STANDARD_ID       0
#define FDCAN_DATA_FRAME        0
#define FDCAN_DLC_BYTES_8       8
#define FDCAN_ESI_ACTIVE        0
#define FDCAN_BRS_ON            1
#define FDCAN_FD_CAN            1
#define FDCAN_NO_TX_EVENTS      0

/* Эмуляция базовых системных функций заглушками */
inline uint8_t HAL_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) { return 0; }
inline void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t PinState) {}
inline void HAL_GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {}
inline void HAL_Delay(uint32_t Delay) {}
inline float SHT35_Read_Temperature_Float(void) { return 22.0f; }
inline void HAL_UART_Receive_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size) {}
inline void HAL_FDCAN_AddMessageToTxFifoQ(FDCAN_HandleTypeDef *hfdcan, FDCAN_TxHeaderTypeDef *pTxHeader, uint8_t *pTxData) {}

/* Эмуляция регистров и системных переменных */
extern uint32_t USART1;
extern uint32_t USART2;
extern uint32_t FDCAN1;
extern uint32_t FDCAN2;

#endif /* MAIN_H */
