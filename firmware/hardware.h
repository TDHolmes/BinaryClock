#ifndef HARDWARE_H__
#define HARDWARE_H__

#include <stdint.h>
#include <avr/io.h>

// Macros for clearing/setting LED0 & LED1
void LED0_set(uint8_t val);   
void LED1_set(uint8_t val);

#define ON  1
#define OFF 0

#define PASS 0
#define FAIL 1

#define F_OSC 8000000UL

#endif /* HARDWARE_H__ */
