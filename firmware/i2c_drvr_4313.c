#include "i2c_drvr_4313.h"
#include "i2c.h"
#include <avr/io.h>
#include <avr/cpufunc.h>
#include <stdint.h>


// private function definitions
void toggle_scl(void);
uint8_t check_ack(void);


// Summary - 
// retval (uint8_t) - 
uint8_t i2c_drvr_init(void)
{
    // set the data reg to all 1's to avoid a start condition
    USIDR = 0xFF;
    // USICR USI Control register
    USICR = 0b00110010;
    // ensure the clock line is deaserted
    if (PORTB & (1 << PORTB7) == 0) {
        toggle_scl();
    }
    return I2C_NO_ERRORS;
}


// Summary - 
// param (uint8_t) byte_to_tx - 
// retval (uint8_t) - 
uint8_t i2c_drvr_start(uint8_t addr_with_mode)
{
    uint8_t retval;
    // generate a start condition (SDA goes high -> low while SCL stays high)
    USIDR = (0 << 7) & 0xFF;
    _NOP();
    // check if the clock is held low by the slave
    while (PINB ^ (1 << PINB7)) {
        _NOP();  // wait a clock cycle
    }
    i2c_drvr_write_byte(addr_with_mode);
    // check ack / nack
    return check_ack();
}


// Summary - 
// retval (uint8_t) - 
uint8_t i2c_drvr_end(void)
{
    // generate a stop condition (SDA goes low -> high while SCL stays high)
    return I2C_NO_ERRORS;
}


// Summary - 
// param (uint8_t) byte_to_tx - 
uint8_t i2c_drvr_write_byte(uint8_t byte_to_tx)
{
    uint8_t i;
    // send byte
    USIDR = byte_to_tx;
    for (i = 7; i != 0; i--) {
        toggle_scl();
        toggle_scl();
    }
    return check_ack();
}


// Summary - 
// retval (uint8_t) - 
uint8_t i2c_drvr_read_byte(uint8_t *byte_read_ptr)
{
    uint8_t i, retval;
    for (i = 7; i != 0; i--) {
        toggle_scl();
        toggle_scl();
    }
    retval = check_ack();
    if (I2C_ACK) {
        // read from i2c buffer
        *byte_read_ptr = USIDR;  // should read from USIBR? (buffer reg)
    }
    return retval;
}

// Summary - 
void toggle_scl(void)
{
    USICR = USICR | 0x01;
}


// Summary - 
// retval (uint8_t) - 
uint8_t check_ack(void)
{
    uint8_t retval;
    // check ack
    toggle_scl();
    // check ack/nack here
    if (PINB ^ (1 << PINB5)) {
        retval = I2C_NO_ERRORS;
    } else {
        retval = I2C_NACK;
    }
    toggle_scl();
    return retval;
}

