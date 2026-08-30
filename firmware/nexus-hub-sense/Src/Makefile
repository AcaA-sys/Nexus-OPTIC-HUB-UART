################################################################################
# ПРОМЫШЛЕННЫЙ MAKEFILE СБОРКИ ПРОШИВКИ СТАНДАРТА N-BUS (v1.3)
# Целевой микроконтроллер: STM32G474RET6 (Ядро Cortex-M4, 170 МГц)
# Репозиторий: Nexus-OPTIC-HUB-UART
################################################################################

# Имя финального файла прошивки
TARGET = nexus_hub_sense_v1.3

# Флаги отладки и оптимизации (Оптимизация -Os по размеру кода для Edge AI)
DEBUG = 0
OPT = -Os

# Пути к исходному коду и заголовочным файлам
BUILD_DIR = build
SRC_DIR = Src
INC_DIR = Inc

# Перечисление Си-файлов для компиляции
C_SOURCES = \
$(SRC_DIR)/hub_main.c

# Настройки компилятора ARM GCC
PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size

HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
# Физические флаги архитектуры кристалла STM32G4 (Cortex-M4 + Hardware FPU)
MCU = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16

# Макросы определений для компилятора (Опорная частота шины N-Bus)
C_DEFS =  \
-DSTM32G474xx \
-DUSE_HAL_DRIVER

# Пути подключения заголовочных файлов (Папка Inc)
C_INCLUDES =  \
-I$(INC_DIR)

# Полный стек флагов компиляции Си-кода (Жесткий контроль синтаксиса -Wall)
CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif

# Флаги генерации зависимостей
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

# Флаги линковщика (Сборка мусора -Wl,--gc-sections для минимизации бинарника)
LDSCRIPT = 
LDFLAGS = $(MCU) -specs=nano.specs $(LDSCRIPT) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--gc-sections

# Главные цели сборки
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

# Магия компиляции: превращаем Си-файлы в объектные .o файлы
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notext some_dummy_var ,$(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	@echo "Компиляция файла: $<"
	@$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS)
	@echo "Линковка бинарного монолита..."
	@$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	@$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	@$(HEX) $< $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	@$(BIN) $< $@	

$(BUILD_DIR):
	@mkdir -p $@

# Очистка папки сборки
clean:
	@rm -fR $(BUILD_DIR)
  
.PHONY: all clean
