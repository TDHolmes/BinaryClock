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
PROGRAMMER = #-c stk500v2 -P avrdoper
SOURCEDIR  = .
BUILDDIR   = 
SOURCES   := $(notdir $(shell find $(SOURCEDIR) -name "*.c"))
#  Get list of object files, with paths
OBJECTS := $(addprefix $(BUILDDIR),$(SOURCES:%.c=%.o))
# OBJECTS    = main.o
FUSES      = -U lfuse:w:0xE4:m
# H:FF, E:DF, L:64
# $(info $$SOURCES is [${SOURCES}])
# $(info $$OBJECTS is [${OBJECTS}])

# ATtiny2313a Fuses
# Extended Byte
# 7 -  (default: )
# 6 -  (default: )
# 5 -  (default: )
# 4 -  (default: )
# 3 -  (default: )
# 2 -  (default: )
# 1 -  (default: )
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
# 2 - CKSEL2: (default: 1 (unprogrammed))
# 1 - CKSEL1: (default: 0 (programmed))
# 0 - CKSEL0: (default: 0 (programmed))
# 

# Tune the lines below only if you know what you are doing:

AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)
COMPILE = avr-gcc -Wextra -O2 -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

# -Os  // size optimization

# symbolic targets:
all:	main.hex

.c.o:
	$(COMPILE) -c $< -o $@
	$(shell find $(SOURCEDIR) -name '*.o' -exec mv {} $(BUILDDIR) \)

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

.c.s:
	$(COMPILE) -S $< -o $@

flash:	all
	$(AVRDUDE) -U flash:w:main.hex:i

fuse:
	$(AVRDUDE) $(FUSES)

# Xcode uses the Makefile targets "", "clean" and "install"
install: flash fuse

# if you use a bootloader, change the command below appropriately:
load: all
	bootloadHID main.hex

clean:
	rm -f main.hex main.elf $(OBJECTS)

# file targets:
main.elf: $(OBJECTS)
	$(COMPILE) -o main.elf $(OBJECTS)

main.hex: main.elf
	rm -f main.hex
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
	avr-size --format=avr --mcu=$(DEVICE) main.elf
# If you have an EEPROM section, you must also create a hex file for the
# EEPROM and add it to the "flash" target.

# Targets for code debugging and analysis:
disasm:	main.elf
	avr-objdump -d main.elf

cpp:
	$(COMPILE) -E main.c
