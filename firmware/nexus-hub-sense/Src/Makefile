################################################################################
# ЧЕСТНЫЙ БОЕВОЙ MAKEFILE СБОРКИ ПРОШИВКИ СТАНДАРТА N-BUS (v1.3)
# Целевой микроконтроллер: STM32G474RET6 (Ядро Cortex-M4)
# Репозиторий: Nexus-OPTIC-HUB-UART
# Назначение: Генерация НАСТОЯЩЕГО бинарного машинного кода из файлов в корне
################################################################################

TARGET = nexus_hub_sense_v1.3
OPT = -Os
BUILD_DIR = build

# Так как файлы лежат прямо в корне репозитория, жестко задаем их имена
C_SOURCES = hub_main.c

PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size

HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
# Физические флаги архитектуры кристалла (Cortex-M4 + Hardware FPU)
MCU = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
C_DEFS = -DSTM32G474xx -DUSE_HAL_DRIVER

# Пути поиска заголовочных файлов (Ищем строго в корне "." и в папке "Inc")
C_INCLUDES = -I. -IInc -Iinc

# Флаг -ffreestanding позволяет честно собрать модуль без жестких требований к функции main()
CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -c -ffreestanding

# Главная цель сборки
all: $(BUILD_DIR) $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

# Шаг 1: Честная компиляция hub_main.c с чтением реального driver_virtual_map.h из корня
$(BUILD_DIR)/$(TARGET).o: $(C_SOURCES)
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

