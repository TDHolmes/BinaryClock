#include "hardware.h"
#include <avr/io.h>
#include <avr/interrupt.h>

// Summary - 
// retval (uint8_t) - 
uint8_t hardware_init(void)
{
    // configure pins for input/output (1 = output, 0 = input)
    PORTB = (1 << PINB5) | (1 << PINB7);  // make sure SDA / SCL don't get asserted
    DDRA = 0x01;  // pin 0 - ind. LED, pin 1 - RTC 1 sec int.
    DDRB = 0xFF;  // all outputs
    DDRD = 0xFE;  // all outputs except UART RX

    // configure peripherals
    // UART_init();
    // i2c_init();
    // timer_init();
    // turn on all interrupts
    // sei();
    return PASS;
}
