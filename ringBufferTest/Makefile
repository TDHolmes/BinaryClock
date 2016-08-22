# Name: Makefile
# Author: <insert your name here>
# Copyright: <insert your copyright message here>
# License: <insert your license reference here>

SOURCEDIR  = .
# build
SOURCES   := $(notdir $(shell find $(SOURCEDIR) -name "*.c"))
#  Get list of object files, with paths
OBJECTS := $(addprefix ,$(SOURCES:%.c=%.o))

# compiler flags and such
CC = gcc
CFLAGS = -Wall -Wextra -Os

# symbolic targets:
all: main.hex
	$(PRINT_INC)

# .c.o:
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.c.s:
	$(CC) $(CFLAGS) -S $< -o $@

# if you use a bootloader, change the command below appropriately:
load: all
	bootloadHID main.hex

clean:
	rm -f main.hex main.elf $(OBJECTS)

# file targets:
main.elf: $(OBJECTS)
	$(CC) $(CFLAGS) -o main.elf $(OBJECTS)

main.hex: main.elf
	rm -f main.hex
cpp:
	$(CC) $(CFLAGS) -E main.c
