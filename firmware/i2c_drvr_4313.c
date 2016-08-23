#include "i2c_drvr_4313.h"
#include "i2c.h"
#include <avr/io.h>
#include <avr/cpufunc.h>
#include <stdint.h>


// private function definitions
void toggle_scl(uint8_t set_val);
retval_t check_ack(void);


#ifdef BITBANG


// Summary - 
// retval (uint8_t) - 
retval_t i2c_drvr_init(void)
{
    // uint8_t retries = 3;
    // // set the data reg to all 1's to avoid a start condition
    // USIDR = 0xFF;
    // // USICR USI Control register
    // // two wire mode, use scl pin for clock and bit USITC to toggle scl
    // USICR = (1 << USIWM1) | (1 << USICS1);
    // // wait a bit for things to settle
    // for (retries = 0; retries < 10; retries++) {
    //     _NOP();
    // }
    // retries = 5;
    // // ensure the clock line is deaserted
    // while ((PINB & (1 << PB7)) == 0) {
    //     toggle_scl();
    //     retries -= 1;
    //     if (retries == 0) {
    //         return I2C_INIT_TIMEOUT;
    //     }
    // }
    return GEN_PASS;
}


// Summary - 
// param (uint8_t) byte_to_tx - 
// retval (uint8_t) - 
retval_t i2c_drvr_start(uint8_t addr_with_mode)
{
    uint8_t i;
    if ((PINB & (1 << PB7)) == 0) {
        return I2C_CLK_LOW_BEFORE_START;
    }
    // generate a start condition (SDA goes high -> low while SCL stays high)
    PORTB = PORTB | (1 << PB5) | (1 << PB7); // put a 1 on SDA & SCL before it's an output
    DDRB = DDRB | (1 << PB5);   // change mode to output
    PORTB = PORTB & (0 << PB5); // bring SDA low
    _NOP();
    DDRB = DDRB | (1 << PB7);   // SCL as output
    // start communication
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
    uint8_t bit;
    // send byte
    // USIDR = byte_to_tx;
    // // deassert scl by clearing start condition flag
    // if (USISR & (1 << (USISIF))) {
    //     USISR &= (~(1 << (USISIF)));
    // } else {
    //     return I2C_START_COND_NOT_DETECTED;
    // }
    for (i = 7; i != 0; i--) {
        bit = (byte_to_tx & 0x80) >> 7; // bring MSB to bit 0
        if (bit == 1) {
            PORTB = PORTB | (1 << PB5); // set SDA to bit value
        } else {
            PORTB = PORTB & ~(1 << PB5); // set SDA to bit value
        }
        toggle_scl();  // clock it in
        byte_to_tx = byte_to_tx << 1; // shift byte
        toggle_scl();
    }
    DDRB = DDRB & ~(1 << PB5); // change SDA back to an input
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
    PORTB ^= (1 << PB7);  // toggle scl
}


// Summary - 
// retval (uint8_t) - 
retval_t check_ack(void)
{
    retval_t retval;
    // check ack
    toggle_scl();
    // check ack/nack here
    if ((PINB & (1 << PB5)) == 0) {  // check if SDA is low
        retval = I2C_ACK;
    } else {
        retval = I2C_NACK;
    }
    toggle_scl();
    return retval;
}


#else /* Non-bitbang version */


// Summary - 
// retval (uint8_t) - 
retval_t i2c_drvr_init(void)
{
    uint8_t retries = 3;
    // set the data reg to all 1's to avoid a start condition
    USIDR = 0xFF;
    // USICR USI Control register
    // two wire mode, use scl pin for clock and bit USITC to toggle scl
    USICR = (1 << USIWM1) | (1 << USICS1);
    // wait a bit for things to settle
    for (retries = 0; retries < 10; retries++) {
        _NOP();
    }
    retries = 5;
    // ensure the clock line is deaserted
    while ((PINB & (1 << PB7)) == 0) {
        toggle_scl();
        retries -= 1;
        if (retries == 0) {
            return I2C_INIT_TIMEOUT;
        }
    }
    return GEN_PASS;
}


// Summary - 
// param (uint8_t) byte_to_tx - 
// retval (uint8_t) - 
retval_t i2c_drvr_start(uint8_t addr_with_mode)
{
    uint8_t i;
    if ((PINB & (1 << PB7)) == 0) {
        return I2C_CLK_LOW_BEFORE_START;
    }
    // generate a start condition (SDA goes high -> low while SCL stays high)

    // start communication
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
    uint8_t bit;
    // send byte
    USIDR = byte_to_tx;
    // deassert scl by clearing start condition flag
    if (USISR & (1 << (USISIF))) {
        USISR &= (~(1 << (USISIF)));
    } else {
        return I2C_START_COND_NOT_DETECTED;
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
    PORTB ^= (1 << PB7);  // toggle scl
}


// Summary - 
// retval (uint8_t) - 
retval_t check_ack(void)
{
    retval_t retval;
    // check ack
    toggle_scl();
    // check ack/nack here
    if ((PINB & (1 << PB5)) == 0) {  // check if SDA is low
        retval = I2C_ACK;
    } else {
        retval = I2C_NACK;
    }
    toggle_scl();
    return retval;
}


#endif
