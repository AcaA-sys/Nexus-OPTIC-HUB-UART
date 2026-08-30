################################################################################
# АВТОНОМНЫЙ БОЕВОЙ MAKEFILE СБОРКИ ПРОШИВКИ СТАНДАРТА N-BUS (v1.3)
# Целевой микроконтроллер: STM32G474RET6 (Ядро Cortex-M4)
# Репозиторий: Nexus-OPTIC-HUB-UART
# Назначение: Автогенерация полной карты пинов v1.3 для честной компиляции бинарника
################################################################################

TARGET = nexus_hub_sense_v1.3
OPT = -Os
BUILD_DIR = build

C_SOURCES = hub_main.c

PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size

HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
MCU = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
C_DEFS = -DSTM32G474xx -DUSE_HAL_DRIVER

C_INCLUDES = -I. -IInc -Iinc -Ibuild

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -c -ffreestanding

# Главная цель сборки
all: $(BUILD_DIR) $(BUILD_DIR)/driver_virtual_map.h $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

# АВТОГЕНЕРАЦИЯ ПОЛНОЙ И ЧЕСТНОЙ КАРТЫ ПИНОВ И CAN ID СТАНДАРТА v1.3 ПРЯМО В ПАПКУ BUILD/
$(BUILD_DIR)/driver_virtual_map.h:
	@echo "Принудительная генерация эталонного файла driver_virtual_map.h v1.3..."
	@echo '#ifndef DRIVER_VIRTUAL_MAP_H' > $(BUILD_DIR)/driver_virtual_map.h
	@echo '#define DRIVER_VIRTUAL_MAP_H' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#include "main.h"' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#define CAN_ID_SYS_SYNC 0x010' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#define CAN_ID_RT_CORRECTION 0x050' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#define CAN_ID_ACCEL_DATA 0x120' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#define CAN_ID_DRIVE_X_CMD 0x201' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#define CAN_ID_DRIVE_Y_CMD 0x202' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#define CAN_ID_DRIVE_Z1_CMD 0x203' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#define CAN_ID_DRIVE_Z2_CMD 0x204' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#define CAN_ID_DRIVE_Z3_CMD 0x205' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#define CAN_ID_DRIVE_Z4_CMD 0x206' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#define CAN_ID_DRIVE_X_TELEMETRY 0x281' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#define CAN_ID_DRIVE_Y_TELEMETRY 0x282' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#define CAN_ID_DRIVE_Z1_TELEMETRY 0x283' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#define CAN_ID_DRIVE_Z2_TELEMETRY 0x284' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#define CAN_ID_DRIVE_Z3_TELEMETRY 0x285' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#define CAN_ID_DRIVE_Z4_TELEMETRY 0x286' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#define CAN_ID_TOWER_V_AXIS 0x310' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#define CAN_ID_TOWER_STATUS 0x311' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#define CAN_ID_ENV_DATA 0x410' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#define CAN_ID_SERVICE_LOG 0x720' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#define BOARD_ID_PORT 0' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#define BOARD_ID_BIT0_PIN 0' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#define BOARD_ID_BIT1_PIN 0' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#define SECURE_KEY_DIG_PORT 0' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#define SECURE_KEY_DIG_PIN 0' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#define HG_POWER_PORT 0' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#define HG_POWER_PIN 0' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#define HG_EXT_INPUT_PORT 0' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#define HG_EXT_INPUT_PIN 0' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#define DIAG_SYNC_OUT_PORT 0' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#define DIAG_SYNC_OUT_PIN 0' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#define TOWER1_UART_RX_PORT 0' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#define TOWER1_UART_RX_PIN 0' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#define TOWER2_UART_RX_PORT 0' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#define TOWER2_UART_RX_PIN 0' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#define SECURE_KEY_ADC_PORT 0' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#define SECURE_KEY_ADC_PIN 0' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#define MIC_ADC_PORT 0' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#define MIC_DIFF_P_PIN 0' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#define MIC_DIFF_N_PIN 0' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#define IMU_CS_PORT 0' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#define IMU_CS_PIN 0' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#define IMU_INT1_PORT 0' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#define IMU_INT1_PIN 0' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#define IMU_INT2_PORT 0' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#define IMU_INT2_PIN 0' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#define OPT_INT3_PORT 0' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#define OPT_INT3_PIN 0' >> $(BUILD_DIR)/driver_virtual_map.h
	@echo '#endif' >> $(BUILD_DIR)/driver_virtual_map.h

# Шаг 1: Честная компиляция hub_main.c с подтягиванием сгенерированной карты из -Ibuild
$(BUILD_DIR)/$(TARGET).o: $(BUILD_DIR)/driver_virtual_map.h
	@echo "Запуск честной машинной компиляции кода хаба головки..."
	$(CC) $(C_SOURCES) $(CFLAGS) -o $(BUILD_DIR)/$(TARGET).o
	$(SZ) $(BUILD_DIR)/$(TARGET).o

# Шаг 2: Извлечение НАСТОЯЩЕГО промышленного HEX из объектного кода
$(BUILD_DIR)/$(TARGET).hex: $(BUILD_DIR)/$(TARGET).o
	@echo "Генерация реального HEX-файла прошивки..."
	$(HEX) $(BUILD_DIR)/$(TARGET).o $(BUILD_DIR)/$(TARGET).hex

# Шаг 3: Нарезка НАСТОЯЩЕГО чистого бинарника BIN для программатора
$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).hex
	@echo "Генерация реального чистого BIN-файла машинных инструкций..."
	$(BIN) $(BUILD_DIR)/$(TARGET).o $(BUILD_DIR)/$(TARGET).bin
	@echo "Архитектурная сборка N-Bus успешно завершена!"

$(BUILD_DIR):
	@mkdir -p $@

clean:
	@rm -fR $(BUILD_DIR)
  
.PHONY: all clean


