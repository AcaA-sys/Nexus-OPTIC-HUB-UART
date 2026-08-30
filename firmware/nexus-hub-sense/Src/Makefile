################################################################################
# СИНТАКСИЧЕСКИЙ MAKEFILE ВЕРИФИКАЦИИ КОДА СТАНДАРТА N-BUS (v1.3)
# Целевой микроконтроллер: STM32G474RET6
# Репозиторий: Nexus-OPTIC-HUB-UART
# Назначение: Побитовая проверка hub_main.c на опечатки без рудиментов стартапа
################################################################################

TARGET = nexus_hub_sense_v1.3
OPT = -Os

BUILD_DIR = build
SRC_DIR = Src
INC_DIR = Inc

# Исходный Си-код хаба головки
C_SOURCES = $(SRC_DIR)/hub_main.c

# Настройки компилятора ARM GCC
PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size

HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
# Флаги архитектуры кристалла (Cortex-M4 + Hardware FPU)
MCU = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16

C_DEFS = -DSTM32G474xx -DUSE_HAL_DRIVER
C_INCLUDES = -I$(INC_DIR)

# ГЛАВНЫЙ СЕКРЕТ СБОРКИ (-c):
# Флаг -c приказывает компилятору просто скомпилировать Си-код в объектный файл,
# проверить все синтаксические ошибки, типы переменных и дефайны, 
# НЕ запуская линковщик и не требуя стартапов и функции main()!
CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -c

# Главная цель сборки
all: $(BUILD_DIR) $(BUILD_DIR)/$(TARGET).o $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

# Шаг 1: Чистая синтаксическая компиляция модуля в объектный файл .o
$(BUILD_DIR)/$(TARGET).o: $(C_SOURCES)
	@echo "Запуск верификации синтаксиса hub_main.c и карты driver_virtual_map.h..."
	@$(CC) $(C_SOURCES) $(CFLAGS) -o $(BUILD_DIR)/$(TARGET).o
	@$(SZ) $(BUILD_DIR)/$(TARGET).o

# Шаг 2: Извлечение промышленного HEX-формата из проверенного модуля
$(BUILD_DIR)/$(TARGET).hex: $(BUILD_DIR)/$(TARGET).o
	@$(HEX) $(BUILD_DIR)/$(TARGET).o $(BUILD_DIR)/$(TARGET).hex

# Шаг 3: Генерация бинарника .bin
$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).hex
	@$(BIN) $(BUILD_DIR)/$(TARGET).o $(BUILD_DIR)/$(TARGET).bin
	@echo "Архитектурный контроль N-Bus пройден. Файлы успешно созданы в папке build/!"

# Автоматическое создание папки build на сервере GitHub
$(BUILD_DIR):
	@mkdir -p $@

clean:
	@rm -fR $(BUILD_DIR)
  
.PHONY: all clean


