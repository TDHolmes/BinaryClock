#include "hardware.h"
#include "ports.h"
#include "global_defines.h"
#include <avr/io.h>
#include <avr/interrupt.h>

// Summary - 
// retval (uint8_t) - 
retval_t hardware_init(void)
{
    // configure pins for input/output (1 = output, 0 = input)
    DDRA = 0x01;  // pin 0 - ind. LED, pin 1 - RTC 1 sec int.
    ANO_PORT = 0xFF; // make sure LEDs don't turn on
    DDRB = (1 << PB6) | (1 << PB4) | (1 << PB3) | (1 << PB2) | (1 << PB1) | (1 << PB0);
    DDRD = 0xFE;  // all outputs except UART RX

    // configure peripherals
    // UART_init();
    // i2c_init();
    // timer_init();
    PCMSK1 = (1 << PCINT9);  // enable a pin change interrupt on the 1 Hz RTC pin 
    GIMSK |= (1 << PCIE1);
    sei();
    return GEN_PASS;
}
