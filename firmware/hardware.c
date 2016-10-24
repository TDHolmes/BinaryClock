#include "hardware.h"
#include "ports.h"
#include "global_defines.h"
#include <avr/io.h>
#include <avr/interrupt.h>

/*! \file hardware.c
    \brief Initialization of basic hardware (port direction & initial pin
    states) goes here in hardware_init().
*/

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

/*!
 * Writes to the internal EEPROM
 * @param[in] address (uint8_t): Address to write to
 * @param[in] data_to_write (uint8_t): Data to write
 */
void EEPROM_write(uint8_t address, uint8_t data_to_write)
{
    // Wait for completion of previous write
    while(EECR & (1<<EEPE));
    cli();  // disable all interrupts
    // Set Programming mode
    EECR = ((0 << EEPM1) | (0 << EEPM0));
    // Set up address and data registers
    EEAR = address;
    EEDR = data_to_write;
    // Write logical one to EEMPE
    EECR |= (1<<EEMPE);
    // Start eeprom write by setting EEPE
    EECR |= (1<<EEPE);
    sei();  // re-enable interrupts
}


/*!
 * Reads from the internal EEPROM
 * @param[in] address_to_read (uint8_t): Address to read from
 * @param[out] data_read (uint8_t): Data read from the address
 */
uint8_t EEPROM_read(uint8_t address_to_read)
{
    // Wait for completion of previous write
    while(EECR & (1<<EEPE));
    cli();  // disable all interrupts
    // Set up address register
    EEAR = address_to_read;
    // Start eeprom read by writing EERE
    EECR |= (1<<EERE);
    sei();  // re-enable interrupts
    // Return data from data register
    return EEDR;
}
