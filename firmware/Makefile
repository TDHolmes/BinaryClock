# Name: Makefile
# Author: <insert your name here>
# Copyright: <insert your copyright message here>
# License: <insert your license reference here>

# This is a prototype Makefile. Modify it according to your needs.
# You should at least check the settings for
# DEVICE ....... The AVR device you compile for
# CLOCK ........ Target AVR clock rate in Hertz
# OBJECTS ...... The object files created from your source files. This list is
#                usually the same as the list of source files with suffix ".o".
# PROGRAMMER ... Options to avrdude which define the hardware you use for
#                uploading to the AVR and the interface where this hardware
#                is connected. We recommend that you leave it undefined and
#                add settings like this to your ~/.avrduderc file:
#                   default_programmer = "stk500v2"
#                   default_serial = "avrdoper"
# FUSES ........ Parameters for avrdude to flash the fuses appropriately.

# /build/

DEVICE     = attiny4313
CLOCK      = 8000000
PROGRAMMER = dragon_isp
SOURCEDIR  = .
BUILDDIR   = build/
# build
SOURCES   := $(notdir $(shell find $(SOURCEDIR) -name "*.c"))
#  Get list of object files, with paths
OBJECTS := $(addprefix $(BUILDDIR),$(SOURCES:%.c=%.o))
# OBJECTS := $(SOURCES:%.c=%.o)
INCLUDE_PATH := /usr/local/CrossPack-AVR-20131216/avr/include/*


# compiler flags and such
CC = avr-gcc
NO_UNUSED_FUNCTS =  -fdata-sections -ffunction-sections -Wl,--gc-sections
CFLAGS = -Wall -Wextra -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) $(NO_UNUSED_FUNCTS) -Os

FUSES  = -U hfuse:w:0xd9:m -U lfuse:w:0xE4:m

# low byte: 0b10100100 = 0xA4

# ATtiny2313a Fuses
# Extended Byte
# 0 - SELFPRGEN: Enables the SPM function (default: 1 [unprogrammed])
# 
# High Byte
# 7 - DWEN: Enables debugWIRE (default: 1 [unprogrammed])
# 6 - EESAVE: Preserves EEPROM memory during Chip Erase operation (default: 1 [unprogrammed])
# 5 - SPIEN: Enables serial programming and downloading of data to device (default: 0 [programmed])
# 4 - WDTON: Sets watchdog timer permanently on (default: 1 [unprogrammed])
# 3 - BODLEVEL2: Sets BOD trigger level (default: 1 [unprogrammed])
# 2 - BODLEVEL1: (default: 1 (unprogrammed))
# 1 - BODLEVEL0: (default: 1 (unprogrammed))
# 0 - RSTDISBL: Disables external reset (default: 1 (unprogrammed))
# 
# Low Byte
# 7 - CKDIV8: Divides clock by 8 (default: 0 (programmed))
# 6 - CKOUT: Outputs system clock on port pin (default: 1 (unprogrammed))
# 5 - SUT1: Sets system start-up time (default: 1 (unprogrammed))
# 4 - SUT0: (default: 0 (programmed))
# 3 - CKSEL3: Selects clock source (default: 0 (programmed))
# 2 - CKSEL2: (default: 0 (programmed))
# 1 - CKSEL1: (default: 1 (unprogrammed))
# 0 - CKSEL0: (default: 0 (programmed))
# 

# Tune the lines below only if you know what you are doing:

AVRDUDE = avrdude -B 20 -c $(PROGRAMMER) -p $(DEVICE)

# symbolic targets:
all: $(BUILDDIR)main.hex

# .c.o:
$(BUILDDIR)%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


$(BUILDDIR)%.o: %.S
	$(CC) $(CFLAGS) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

.c.s:
	$(CC) $(CFLAGS) -S $< -o $@

flash:	all
	$(AVRDUDE) -U flash:w:$(BUILDDIR)main.hex:i

fuse:
	$(AVRDUDE) $(FUSES)

# Xcode uses the Makefile targets "", "clean" and "install"
install: flash fuse

# if you use a bootloader, change the command below appropriately:
load: all
	bootloadHID $(BUILDDIR)main.hex

clean:
	rm -f $(BUILDDIR)main.hex $(BUILDDIR)main.elf $(OBJECTS)

# file targets:
$(BUILDDIR)main.elf: $(OBJECTS)
	$(CC) $(CFLAGS) -o $(BUILDDIR)main.elf $(OBJECTS)

$(BUILDDIR)main.hex: $(BUILDDIR)main.elf
	rm -f $(BUILDDIR)main.hex
	avr-objcopy -j .text -j .data -O ihex $(BUILDDIR)main.elf $(BUILDDIR)main.hex
	avr-size --format=avr --mcu=$(DEVICE) $(BUILDDIR)main.elf
# If you have an EEPROM section, you must also create a hex file for the
# EEPROM and add it to the "flash" target.

# Targets for code debugging and analysis:
disasm:	$(BUILDDIR)main.elf
	avr-objdump -d $(BUILDDIR)main.elf

cpp:
	$(CC) $(CFLAGS) -E main.c
