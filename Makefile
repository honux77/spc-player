# Makefile for SNES SPC Player

# Tools
CC65_PATH ?= /usr/local
CC = $(CC65_PATH)/bin/cc65
AS = $(CC65_PATH)/bin/ca65
LD = $(CC65_PATH)/bin/ld65
HOST_CC = gcc

# Directories
SRC_DIR = src
INCLUDE_DIR = include
TOOLS_DIR = tools
DATA_DIR = data
BUILD_DIR = build

# Output
ROM = $(BUILD_DIR)/spc-player.sfc

# Source files
C_SOURCES = $(SRC_DIR)/main.c $(SRC_DIR)/snes.c $(SRC_DIR)/spc_player.c
ASM_SOURCES = $(SRC_DIR)/init.s
SPC_DATA = $(BUILD_DIR)/spc_data.c

# Object files
C_OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(C_SOURCES))
ASM_OBJECTS = $(patsubst $(SRC_DIR)/%.s,$(BUILD_DIR)/%.o,$(ASM_SOURCES))
DATA_OBJECTS = $(BUILD_DIR)/spc_data.o

ALL_OBJECTS = $(C_OBJECTS) $(ASM_OBJECTS) $(DATA_OBJECTS)

# Tools
SPC2BIN = $(BUILD_DIR)/spc2bin
SPCBUNDLE = $(BUILD_DIR)/spcbundle

# SPC files (add your .spc files to data/ directory)
SPC_FILES = $(wildcard $(DATA_DIR)/*.spc)
SPC_BIN_FILES = $(patsubst $(DATA_DIR)/%.spc,$(BUILD_DIR)/%.spc.bin,$(SPC_FILES))

# Flags
CFLAGS = -t none -O -I $(INCLUDE_DIR)
ASFLAGS = -I $(SRC_DIR)
LDFLAGS = -C lorom.cfg

# Default target
.PHONY: all
all: $(ROM)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Build converter tools
$(SPC2BIN): $(TOOLS_DIR)/spc2bin.c | $(BUILD_DIR)
	$(HOST_CC) -o $@ $< -O2 -Wall

$(SPCBUNDLE): $(TOOLS_DIR)/spcbundle.c | $(BUILD_DIR)
	$(HOST_CC) -o $@ $< -O2 -Wall

# Convert SPC files to binary
$(BUILD_DIR)/%.spc.bin: $(DATA_DIR)/%.spc $(SPC2BIN)
	$(SPC2BIN) $< $@

# Bundle SPC binaries into C source
$(SPC_DATA): $(SPC_BIN_FILES) $(SPCBUNDLE)
ifeq ($(SPC_BIN_FILES),)
	@echo "Warning: No SPC files found in $(DATA_DIR)/. Creating empty bundle."
	@echo '#include "spc_player.h"' > $(SPC_DATA)
	@echo 'const spc_data_t spc_tracks[] = {{}};' >> $(SPC_DATA)
	@echo 'const uint8_t spc_track_count = 0;' >> $(SPC_DATA)
else
	$(SPCBUNDLE) $@ $(SPC_BIN_FILES)
endif

# Compile C sources
$(BUILD_DIR)/%.s: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $<

# Compile C data
$(BUILD_DIR)/spc_data.s: $(SPC_DATA)
	$(CC) $(CFLAGS) -o $@ $<

# Assemble
$(BUILD_DIR)/%.o: $(BUILD_DIR)/%.s
	$(AS) $(ASFLAGS) -o $@ $<

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.s | $(BUILD_DIR)
	$(AS) $(ASFLAGS) -o $@ $<

# Link
$(ROM): $(ALL_OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $(ALL_OBJECTS)
	@echo "ROM created: $@"
	@ls -lh $@

# Clean
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

# Help
.PHONY: help
help:
	@echo "SNES SPC Player Makefile"
	@echo ""
	@echo "Targets:"
	@echo "  all        - Build ROM (default)"
	@echo "  clean      - Remove build files"
	@echo "  help       - Show this help"
	@echo ""
	@echo "Usage:"
	@echo "  1. Place .spc files in $(DATA_DIR)/ directory"
	@echo "  2. Run 'make' to build ROM"
	@echo "  3. ROM will be created at $(ROM)"
	@echo ""
	@echo "Requirements:"
	@echo "  - cc65 toolchain installed"
	@echo "  - Set CC65_PATH if not in /usr/local"

.PHONY: test-tools
test-tools: $(SPC2BIN) $(SPCBUNDLE)
	@echo "Testing converter tools..."
	@echo "SPC2BIN: $$(test -f $(SPC2BIN) && echo 'OK' || echo 'FAIL')"
	@echo "SPCBUNDLE: $$(test -f $(SPCBUNDLE) && echo 'OK' || echo 'FAIL')"
