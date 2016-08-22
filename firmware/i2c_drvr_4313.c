#include "i2c_drvr_4313.h"
#include "i2c.h"
#include <avr/io.h>
#include <avr/cpufunc.h>
#include <stdint.h>


// private function definitions
void toggle_scl(void);
retval_t check_ack(void);


// Summary - 
// retval (uint8_t) - 
retval_t i2c_drvr_init(void)
{
    // set the data reg to all 1's to avoid a start condition
    USIDR = 0xFF;
    // USICR USI Control register
    USICR = 0b00110010;
    // ensure the clock line is deaserted
    if (PINB & ((1 << PB7) == 0)) {
        toggle_scl();
    }
    return GEN_PASS;
}


// Summary - 
// param (uint8_t) byte_to_tx - 
// retval (uint8_t) - 
retval_t i2c_drvr_start(uint8_t addr_with_mode)
{
    // uint8_t retries = 255;
    // generate a start condition (SDA goes high -> low while SCL stays high)
    USIDR = (0 << 7) & 0xFF;
    _NOP();
    // check if the clock is held low by the slave
    while (~PINB & (1 << PINB7)) {
        _NOP();  // wait a clock cycle
        _NOP();  // wait a clock cycle
        _NOP();  // wait a clock cycle
        _NOP();  // wait a clock cycle
        _NOP();  // wait a clock cycle
        // // check retry count
        // if (retries == 0) {
        //     return I2C_TIMEOUT;
        // } else {
        //     retries -= 1;
        // }
    }
    i2c_drvr_write_byte(addr_with_mode);
    // check ack / nack
    return check_ack();
}


// Summary - 
// retval (uint8_t) - 
retval_t i2c_drvr_end(void)
{
    // generate a stop condition (SDA goes low -> high while SCL stays high)
    USIDR = 0xFF;
    return GEN_PASS;
}


// Summary - 
// param (uint8_t) byte_to_tx - 
retval_t i2c_drvr_write_byte(uint8_t byte_to_tx)
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
retval_t i2c_drvr_read_byte(uint8_t *byte_read_ptr)
{
    uint8_t i;
    retval_t retval;
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
retval_t check_ack(void)
{
    retval_t retval;
    // check ack
    toggle_scl();
    // check ack/nack here
    if (~PINB & (1 << PINB5)) {  // check if SDA is low
        retval = I2C_ACK;
    } else {
        retval = I2C_NACK;
    }
    toggle_scl();
    return retval;
}

