##############################################################################
# STM32F103 + StdPeriph + arm-none-eabi-gcc Makefile
#
# Build:   make
# Flash:   make flash
# Clean:   make clean
#
# Override toolchain location (optional):
#   make GCC_PATH=/path/to/arm-gnu-toolchain/bin
##############################################################################


##############################################################################
# Project configuration
##############################################################################

# Output binary name (without extension). Final files: build/<TARGET>.elf/.hex/.bin
TARGET = my_stm32_project

# 1 = include debug symbols (-g), 0 = strip them
DEBUG = 1

# Optimization level:
#   -O0  no optimization, easiest to debug
#   -Og  optimize for debugging (recommended during development)
#   -Os  optimize for size (recommended for release)
#   -O2  optimize for speed
OPT = -Og

# All build artifacts (.o, .d, .elf, .hex, .bin, .map) go here
BUILD_DIR = build


##############################################################################
# Source files
##############################################################################

# C sources: list every .c that needs to be compiled.
# Add new StdPeriph drivers here when you start using more peripherals
# (e.g. stm32f10x_usart.c, stm32f10x_tim.c, ...).
C_SOURCES = \
User/main.c \
User/stm32f10x_it.c \
Hardware/Buzzer.c \
Hardware/LightSensor.c \
System/Delay.c \
Library/system_stm32f10x.c \
Library/misc.c \
Library/stm32f10x_gpio.c \
Library/stm32f10x_rcc.c

# Assembly sources. The startup file sets up the vector table, stack pointer,
# and jumps to Reset_Handler -> SystemInit() -> main().
# Pick the one matching your chip density:
#   _ld = low density, _md = medium density (F103C8/CB),
#   _hd = high density, _xl = XL, _cl = connectivity line
ASM_SOURCES = Start/startup_stm32f10x_md.s


##############################################################################
# Toolchain (arm-none-eabi-*)
##############################################################################

PREFIX = arm-none-eabi-

# If GCC_PATH is set on the command line, use that toolchain;
# otherwise rely on the system PATH.
ifdef GCC_PATH
  CC = $(GCC_PATH)/$(PREFIX)gcc
  AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
  CP = $(GCC_PATH)/$(PREFIX)objcopy
  SZ = $(GCC_PATH)/$(PREFIX)size
else
  CC = $(PREFIX)gcc
  AS = $(PREFIX)gcc -x assembler-with-cpp
  CP = $(PREFIX)objcopy
  SZ = $(PREFIX)size
endif

# objcopy invocations for producing flashable formats from the ELF
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S


##############################################################################
# MCU / architecture flags
##############################################################################

# Cortex-M3 core (STM32F1 family)
CPU = -mcpu=cortex-m3

# F1 has no FPU, leave FPU and FLOAT-ABI empty.
# When porting to F4/F7 (which have an FPU), set:
#   FPU       = -mfpu=fpv4-sp-d16
#   FLOAT-ABI = -mfloat-abi=hard
FPU       =
FLOAT-ABI =

# -mthumb: emit Thumb-2 instructions (Cortex-M only runs Thumb)
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)


##############################################################################
# Preprocessor defines
##############################################################################

# C defines:
#   STM32F10X_MD          tells stm32f10x.h which density variant we are on
#                         (LD / MD / HD / XL / CL — match your chip)
#   USE_STDPERIPH_DRIVER  enables the StdPeriph driver layer
#   HSE_VALUE             external crystal frequency in Hz
#                         (Blue Pill = 8 MHz; change if your board differs)
C_DEFS = \
-DSTM32F10X_MD \
-DUSE_STDPERIPH_DRIVER \
-DHSE_VALUE=8000000


##############################################################################
# Include paths
##############################################################################

# -I<dir> tells the preprocessor where to look for #include "..." headers.
# Flat layout: all StdPeriph + CMSIS headers live under library/.
C_INCLUDES = \
-IUser \
-ILibrary \
-IHardware \
-ISystem


##############################################################################
# Compiler flags
##############################################################################

# Assembler flags (kept minimal — the startup file does not need much)
ASFLAGS = $(MCU) $(OPT) -Wall -fdata-sections -ffunction-sections

# C compiler flags:
#   -Wall                       enable common warnings
#   -fdata-sections             put each variable in its own section
#   -ffunction-sections         put each function in its own section
#   (combined with -Wl,--gc-sections at link time, this strips unused code)
CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall \
         -fdata-sections -ffunction-sections

# Add debug info if DEBUG=1
ifeq ($(DEBUG), 1)
  CFLAGS += -g -gdwarf-2
endif

# Auto-generate header dependency files (.d) alongside each .o.
#   -MMD  emit dependency info, ignoring system headers
#   -MP   add phony targets so deleting a header doesn't break the build
#   -MF   write the .d file next to the .o
# These .d files are -include'd at the bottom, so editing any header
# correctly triggers a rebuild of every .c that uses it.
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


##############################################################################
# Linker flags
##############################################################################

# Linker script: describes the memory layout (FLASH/RAM addresses and sizes).
LDSCRIPT = Linker/STM32F103XX_FLASH.ld

# Standard libraries:
#   -lc       newlib C library
#   -lm       math library
#   -lnosys   stub syscalls (write/read/etc.) — required for bare-metal
LIBS = -lc -lm -lnosys

# Linker flags:
#   -specs=nano.specs   use newlib-nano (much smaller than full newlib)
#   -T$(LDSCRIPT)       use our linker script
#   -Wl,-Map=...,--cref produce a memory map file for inspection
#   -Wl,--gc-sections   discard unused sections (pairs with -ffunction-sections)
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBS) \
          -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref \
          -Wl,--gc-sections


##############################################################################
# Default target
##############################################################################

# Running plain `make` builds elf, hex and bin in one go.
all: $(BUILD_DIR)/$(TARGET).elf \
     $(BUILD_DIR)/$(TARGET).hex \
     $(BUILD_DIR)/$(TARGET).bin


##############################################################################
# Object list + vpath
##############################################################################

# Convert each source path into build/<basename>.o so all objects land
# flat inside build/, regardless of where the .c lives.
OBJECTS  = $(addprefix $(BUILD_DIR)/, $(notdir $(C_SOURCES:.c=.o)))
OBJECTS += $(addprefix $(BUILD_DIR)/, $(notdir $(ASM_SOURCES:.s=.o)))

# vpath tells make which directories may contain source files matching
# a given pattern. Without this, the pattern rules below couldn't find
# the .c / .s files that live in subdirectories.
vpath %.c $(sort $(dir $(C_SOURCES)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))


##############################################################################
# Build rules
##############################################################################

# Compile each .c into build/xxx.o.
# Listing `Makefile` as a prerequisite forces a full rebuild whenever the
# Makefile itself changes (e.g. you tweak a flag or add a -D define).
$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) $< -o $@

# Assemble each .s into build/xxx.o
$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(ASFLAGS) $< -o $@

# Link all objects into the final ELF, then print its size.
$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

# Convert ELF to Intel HEX (used by some flashers and bootloaders)
$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@

# Convert ELF to raw binary (smallest, what gets written directly to flash)
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@

# Create the build directory if it doesn't exist yet.
# Listed as an order-only prerequisite (after `|`) so its timestamp
# does not trigger spurious rebuilds.
$(BUILD_DIR):
	mkdir $@


##############################################################################
# Flash via OpenOCD (ST-Link)
##############################################################################

# Adjust interface file if you use a different probe:
#   stlink.cfg     ST-Link V2 / V2-1 / V3 (most common)
#   jlink.cfg      Segger J-Link
flash: all
	openocd -f interface/stlink.cfg -f target/stm32f1x.cfg \
	        -c "program $(BUILD_DIR)/$(TARGET).elf verify reset exit"


##############################################################################
# Clean
##############################################################################

clean:
	-rm -fR $(BUILD_DIR)


##############################################################################
# Header dependency files
##############################################################################

# Pull in the .d files generated by -MMD so make knows which .c depends
# on which .h. The leading `-` makes make ignore the case where no .d
# files exist yet (i.e. the very first build).
-include $(wildcard $(BUILD_DIR)/*.d)


##############################################################################
# Phony targets (these are not real files)
##############################################################################
.PHONY: all clean flash