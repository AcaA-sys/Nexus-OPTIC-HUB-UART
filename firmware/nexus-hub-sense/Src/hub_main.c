/**
  ******************************************************************************
  * @file    hub_main.c
  * @brief   Ядро диспетчеризации Edge AI измерительного хаба головки
  ******************************************************************************
  */

#include "driver_virtual_map.h"

/* Глобальные переменные статуса платформы */
uint8_t  nexus_board_hardware_id = 0;
bool     boot_security_key_passed = false;
float    last_stable_temperature_c = 22.0f;

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
  * @note   Вызывается в первые миллисекунды после сброса MCU до пуска N-Bus движения!
  */
void Nexus_Hub_Verify_Security_Lock(void)
{
    // 1. Принудительно выставляем порты в режим ввода без подтяжек (Hi-Z)
    // Конденсатор C51 (1мкФ) начинает медленный заряд через R46 (1МОм)
    
    // 2. Делаем экспресс-проверку цифрового бита PC0
    if (HAL_GPIO_ReadPin(SECURE_KEY_DIG_PORT, SECURE_KEY_DIG_PIN) == GPIO_PIN_RESET) 
    {
        // Конденсатор еще разряжен, потенциал близок к 0В. Проверка начальной фазы пройдена.
        boot_security_key_passed = true;
    }
    else 
    {
        // Конденсатор уже заряжен или линия принудительно подтянута извне программатором.
        // Несанкционированный доступ — аппаратная блокировка Flash.
        boot_security_key_passed = false;
        while(1) {  
            // Жесткий останов ядра, изоляция памяти от чтения
        }
    }
}

/**
  * @brief  Аппаратное переключение питания лазера Panasonic HG-C1030
  */
void Nexus_Hub_Laser_Control(bool enable)
{
    if (enable) {
        HAL_GPIO_WritePin(HG_POWER_PORT, HG_POWER_PIN, GPIO_PIN_SET);   // Включаем лазер
        HAL_Delay(50); // Паспортное время стабилизации луча диода
    } else {
        HAL_GPIO_WritePin(HG_POWER_PORT, HG_POWER_PIN, GPIO_PIN_RESET); // Отключаем лазер
    }
}

/**
  * @brief  Импульсный сброс нуля лазера по внешней CTP-матрице (External Input)
  */
void Nexus_Hub_Laser_Trigger_Zero_Set(void)
{
    // Для фиксации Zero Set требуется удержание линии LOW от 50 до 100 мс
    HAL_GPIO_WritePin(HG_CTRL_PORT, HG_EXT_INPUT_PIN, GPIO_PIN_RESET);
    HAL_Delay(60);
    HAL_GPIO_WritePin(HG_CTRL_PORT, HG_EXT_INPUT_PIN, GPIO_PIN_SET);
}
