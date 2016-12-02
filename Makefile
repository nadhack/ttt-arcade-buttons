# C source files (the order of these files is important)
C_SRC = \
 app-input.c \
 app-sequencer.c \
 app-serial.c \
 main.c

# Assembly source files
AS_SRC =


# output name (no extension)
OUTPUT = buttons

# output format (binary, srec or ihex)
FORMAT = binary

# optimization level (0, 1, 2, 3 or s)
OPTIMIZATION_LEVEL = s

# target microcontroller
MCU = atmega328p

# target microcontroller clock frequency (Hz)
MCU_FREQ = 16000000

# programmer flags (for fuse, eeprom and flash programming)
PROG_COMMON_FLAGS = -c avrispmkII -P usb
PROG_COMMON_FLAGS += -p m328p

# programmer flags for flash programming
PROG_FLASH_FLAGS = -B 2

# programmer flags for fuse programming
PROG_FUSE_FLAGS = -B 16
PROG_FUSE_FLAGS += -U efuse:w:0x07:m
PROG_FUSE_FLAGS += -U hfuse:w:0xC9:m
PROG_FUSE_FLAGS += -U lfuse:w:0xFC:m

# toolchain prefix
TOOLCHAIN_PREFIX = avr-

# tools
export AR = $(TOOLCHAIN_PREFIX)ar
export AS = $(TOOLCHAIN_PREFIX)as
export CC = $(TOOLCHAIN_PREFIX)gcc
export COPY = cp
export LD = $(TOOLCHAIN_PREFIX)ld
export NM = $(TOOLCHAIN_PREFIX)nm
export OBJCOPY = $(TOOLCHAIN_PREFIX)objcopy
export OBJDUMP = $(TOOLCHAIN_PREFIX)objdump
export RANLIB = $(TOOLCHAIN_PREFIX)ranlib
export REMOVE = rm -f
export SHELL = /bin/sh
export SIZE = $(TOOLCHAIN_PREFIX)size
export PROG = avrdude
export HOST_CC = gcc

# Symbols for which to force linkage
FORCE_LINK =
 
# Include flags
INCLUDES = \
 -I.

# Common flags
COMMON = -mmcu=$(MCU)

# C compiler flags
CFLAGS = \
 $(COMMON) \
 $(INCLUDES) \
 -Wall -gdwarf-2 \
 -fsigned-char -fpack-struct -fshort-enums -funsigned-bitfields \
 -O$(OPTIMIZATION_LEVEL) \
 -D F_CPU=$(MCU_FREQ) \
 -std=c99

# compiler flags for generating dependency flags
GENDEPFLAGS = -MD -MP -MF .dep/$(@F).d

# Linker flags
LDFLAGS = \
 $(COMMON) \
 $(foreach symbol,$(FORCE_LINK),-u $(symbol)) 

# Listing files
LST = $(SRC:.c=.lst)

# files to delete when cleaning
CLEAN_FILES= \
 $(OUTPUT).bin \
 $(OUTPUT).eep \
 $(OUTPUT).obj \
 $(OUTPUT).cof \
 $(OUTPUT).elf \
 $(OUTPUT).map \
 $(OUTPUT).a90 \
 $(OUTPUT).sym \
 $(OUTPUT).lnk \
 $(OUTPUT).lss \
 $(LST) \
 .dep/*

# rules
all: build

build: elf bin lss sym size

elf: $(OUTPUT).elf
bin: $(OUTPUT).bin
lss: $(OUTPUT).lss
sym: $(OUTPUT).sym

size: $(OUTPUT).elf
	$(SIZE) -B $(OUTPUT).elf

%.bin: $(OUTPUT).elf
	$(OBJCOPY) -O $(FORMAT) $< $@

%.lss: $(OUTPUT).elf
	$(OBJDUMP) -h -S -C $< > $@

%.sym: $(OUTPUT).elf
	$(NM) -n $< > $@

.SECONDARY: $(OUTPUT).elf
%.elf: $(C_SRC)
	$(CC) $(LDFLAGS) $(CFLAGS) $(C_SRC) -o $@

clean:
	$(REMOVE) $(strip $(CLEAN_FILES))

program: $(OUTPUT).bin
	$(PROG) $(PROG_COMMON_FLAGS) $(PROG_FLASH_FLAGS) -u -U flash:w:$<

program_fuses:
	$(PROG) $(PROG_COMMON_FLAGS) $(PROG_FUSE_FLAGS)

program_all: program_fuses program

erase:
	$(PROG) $(PROG_COMMON_FLAGS) -e

reset:
	$(PROG) $(PROG_COMMON_FLAGS) -F noreset

# include dependency rules
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

# phony targets
.PHONY: all build elf bin lss sym size clean program program_fuses program_all erase reset
