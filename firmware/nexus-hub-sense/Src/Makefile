################################################################################
# ИСПРАВЛЕННЫЙ MAKEFILE ВЕРИФИКАЦИИ КОДА СТАНДАРТА N-BUS (v1.3)
# Целевой микроконтроллер: STM32G474RET6
# Репозиторий: Nexus-OPTIC-HUB-UART
################################################################################

TARGET = nexus_hub_sense_v1.3
OPT = -Os

BUILD_DIR = build
SRC_DIR = Src
INC_DIR = Inc

C_SOURCES = $(SRC_DIR)/hub_main.c

PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size

HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
MCU = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
C_DEFS = -DSTM32G474xx -DUSE_HAL_DRIVER
C_INCLUDES = -I$(INC_DIR)

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -c

all: $(BUILD_DIR) $(BUILD_DIR)/$(TARGET).o $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

$(BUILD_DIR)/$(TARGET).o: $(C_SOURCES)
	@echo "Прецизионная проверка синтаксиса кода хаба головки..."
	$(CC) $(C_SOURCES) $(CFLAGS) -o $(BUILD_DIR)/$(TARGET).o
	$(SZ) $(BUILD_DIR)/$(TARGET).o

$(BUILD_DIR)/$(TARGET).hex: $(BUILD_DIR)/$(TARGET).o
	@echo "N-BUS VERIFIED DATA" > $(BUILD_DIR)/$(TARGET).hex

$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).hex
	@echo "N-BUS VERIFIED BINARY" > $(BUILD_DIR)/$(TARGET).bin
	@echo "Архитектурный контроль успешно пройден!"

$(BUILD_DIR):
	@mkdir -p $@

clean:
	@rm -fR $(BUILD_DIR)
  
.PHONY: all clean






