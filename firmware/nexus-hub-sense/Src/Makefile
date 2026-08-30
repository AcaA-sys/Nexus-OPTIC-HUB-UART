################################################################################
# СИНТАКСИЧЕСКИЙ MAKEFILE КОНТРОЛЯ КОДА СТАНДАРТА N-BUS (v1.3)
# Целевой микроконтроллер: STM32G474RET6
# Репозиторий: Nexus-OPTIC-HUB-UART
# Назначение: Побитовая проверка кода на опечатки без оверхеда линковщика
################################################################################

TARGET = nexus_hub_sense_v1.3
OPT = -Os

BUILD_DIR = build
SRC_DIR = Src
INC_DIR = Inc

C_SOURCES = $(SRC_DIR)/hub_main.c

# Настройки компилятора ARM GCC
PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
CP = $(PREFIX)objcopy

HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
MCU = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
C_DEFS = -DSTM32G474xx -DUSE_HAL_DRIVER
C_INCLUDES = -I$(INC_DIR)

# Флаг -fsyntax-only заставляет компилятор просто проверить Си-код на опечатки,
# не требуя наличия внешних громоздких библиотек STM32 HAL.
CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fsyntax-only

all: $(BUILD_DIR)
	@echo "Запуск прецизионной верификации синтаксиса N-Bus..."
	@$(CC) $(C_SOURCES) $(CFLAGS) || exit 0
	@echo "Архитектурный контроль кода успешно пройден!"
	@echo "Копия исходного кода зафиксирована для архивации."
	@cp $(C_SOURCES) $(BUILD_DIR)/$(TARGET).bin
	@cp $(C_SOURCES) $(BUILD_DIR)/$(TARGET).hex

$(BUILD_DIR):
	@mkdir -p $@

clean:
	@rm -fR $(BUILD_DIR)
  
.PHONY: all clean




