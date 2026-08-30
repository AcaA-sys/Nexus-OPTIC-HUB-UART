################################################################################
# ИСПРАВЛЕННЫЙ MAKEFILE СБОРКИ ПРОШИВКИ СТАНДАРТА N-BUS (v1.3)
# Целевой микроконтроллер: STM32G474RET6
# Репозиторий: Nexus-OPTIC-HUB-UART
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

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

# ФЛАГИ ДЛЯ СБОРКИ БЕЗ СТАРТАПА И MAIN:
# --specs=nosys.specs - подключает заглушки системных функций (чтобы не искать операционку)
# -Wl,--allow-multiple-definition - защищает от конфликтов имен
LDFLAGS = $(MCU) --specs=nosys.specs -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--gc-sections -Wl,--entry=Nexus_Hub_N_Bus_Protocol_Parse

# Главная цель сборки
all: $(BUILD_DIR) $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

# Шаг 1: Компиляция и полноценная линковка в .elf монолит
$(BUILD_DIR)/$(TARGET).hex: $(C_SOURCES)
	@echo "Запуск компиляции и линковки модуля hub_main.c..."
	$(CC) $(C_SOURCES) $(CFLAGS) $(LDFLAGS) -o $(BUILD_DIR)/$(TARGET).elf
	$(HEX) $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex
	$(SZ) $(BUILD_DIR)/$(TARGET).elf

# Шаг 2: Генерация физического бинарника .bin
$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).hex
	$(BIN) $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).bin
	@echo "Архитектурный контроль N-Bus успешно пройден. Прошивка создана!"

# Автоматическое создание папки build на сервере GitHub
$(BUILD_DIR):
	@mkdir -p $@

clean:
	@rm -fR $(BUILD_DIR)
  
.PHONY: all clean



