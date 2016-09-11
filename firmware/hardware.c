#include "hardware.h"
#include "ports.h"
#include "global_defines.h"
#include <avr/io.h>
#include <avr/interrupt.h>

/*!
 * Initializes all the hardware
 * @param[out] retval (retval_t): The memory area to copy to.
 */
retval_t hardware_init(void)
{
    // configure pins for input/output (1 = output, 0 = input)
    DDRA = 0x00;  // pin 0 - ind. LED, pin 1 - RTC 1 sec int.
    ANO_PORT = 0xFF; // make sure LEDs don't turn on
    DDRB = (1 << PB6) | (1 << PB4) | (1 << PB3) | (1 << PB2) | (1 << PB1) | (1 << PB0);
    DDRD = 0xFE;  // all outputs except UART RX
    sei();
    return GEN_PASS;
}
