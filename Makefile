# Makefile - Projeto Final Assert
# Reference makefile for STM32 firmware compilation
# Note: This is a reference. Use STM32CubeIDE or STM32CubeMX generated Makefile in production

# Project Settings
PROJECT_NAME = Projeto_Final
BUILD_DIR = build
SRC_DIR = Src
INC_DIR = Inc

# Compiler Settings
CC = arm-none-eabi-gcc
CFLAGS = -c -Wall -O2 -fno-common -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard
CFLAGS += -I$(INC_DIR)

# Linker Settings
LD = arm-none-eabi-gcc
LDFLAGS = -mthumb -mcpu=cortex-m4

# Objcopy for binary generation
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump

# Source files
C_SOURCES = \
    $(SRC_DIR)/main.c \
    $(SRC_DIR)/Bsp.c \
    $(SRC_DIR)/Sampler.c \
    $(SRC_DIR)/LedPwm.c \
    $(SRC_DIR)/SerialCmd.c \
    $(SRC_DIR)/Button.c

# Object files
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(C_SOURCES))

# Output files
ELF_FILE = $(BUILD_DIR)/$(PROJECT_NAME).elf
HEX_FILE = $(BUILD_DIR)/$(PROJECT_NAME).hex
BIN_FILE = $(BUILD_DIR)/$(PROJECT_NAME).bin

# Default target
all: $(BIN_FILE)

# Create build directory
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Compile C files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

# Link objects into ELF
$(ELF_FILE): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^
	@echo "Built: $(ELF_FILE)"

# Generate HEX file
$(HEX_FILE): $(ELF_FILE)
	$(OBJCOPY) -O ihex $(ELF_FILE) $(HEX_FILE)
	@echo "Built: $(HEX_FILE)"

# Generate BIN file
$(BIN_FILE): $(ELF_FILE)
	$(OBJCOPY) -O binary $(ELF_FILE) $(BIN_FILE)
	@echo "Built: $(BIN_FILE)"

# Display symbols
dump:
	$(OBJDUMP) -h $(ELF_FILE)

# Clean build directory
clean:
	rm -rf $(BUILD_DIR)
	@echo "Cleaned: $(BUILD_DIR)"

# Rebuild
rebuild: clean all

# Documentation
docs:
	doxygen Doxyfile
	@echo "Documentation generated in Doxygen/html/"

# Help
help:
	@echo "Makefile targets:"
	@echo "  make                - Build all"
	@echo "  make clean          - Remove build directory"
	@echo "  make rebuild        - Clean and build"
	@echo "  make docs           - Generate documentation"
	@echo "  make dump           - Show ELF symbols"
	@echo "  make help           - Show this help"

.PHONY: all clean rebuild docs dump help

# Notes:
# - This Makefile is for reference only
# - Use STM32CubeIDE for full development
# - STM32CubeMX will generate optimized Makefile with HAL libraries
# - For cross-compilation, ensure arm-none-eabi toolchain is installed
