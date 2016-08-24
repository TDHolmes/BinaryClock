#include "i2c_drvr_4313.h"
#include "i2c.h"
#include <avr/io.h>
#include <avr/cpufunc.h>
#include <stdint.h>


// private function definitions
retval_t check_ack(void);

#ifdef I2C_BITBANG

    // define the set pin functions for bitbang
    void set_SDA(uint8_t val);
    void set_SCL(uint8_t val);
    uint8_t read_SDA(void);
    void write_ack(uint8_t val);


    void set_SDA(uint8_t val)
    {
        if (val != 0) {
            PORTB = PORTB | (1 << PB5);  // set port to 1
            DDRB = DDRB & ~(1 << PB5);   // change mode to input
        } else {
            PORTB = PORTB & ~(1 << PB5); // set port to 0
            DDRB = DDRB | (1 << PB5);    // change mode to output
        }
    }

    uint8_t read_SDA(void)
    {
        uint8_t retval;
        // make sure SDA is in read mode
        DDRB = DDRB & ~(1 << PB5);   // change mode to input
        _NOP();
        retval = (PINB & (1 << PB5)) >> PB5; // read bit
        return retval;
    }

    void set_SCL(uint8_t val)
    {
        if (val != 0) {
            PORTB = PORTB | (1 << PB7);  // set port to 1
            DDRB = DDRB & ~(1 << PB7);   // change mode to input
        } else {
            PORTB = PORTB & ~(1 << PB7); // set port to 0
            DDRB = DDRB | (1 << PB7);    // change mode to output
        }
    }

    // Summary - 
    // retval (retval_t) - 
    retval_t i2c_drvr_init(void)
    {
        set_SCL(1);
        set_SDA(1);
        return GEN_PASS;
    }


    // Summary - 
    // param (uint8_t) byte_to_tx - 
    // retval (retval_t) - 
    retval_t i2c_drvr_start(uint8_t addr_with_mode)
    {
        uint8_t i;

        if ((PINB & (1 << PB7)) == 0) {
            return I2C_CLK_LOW_BEFORE_START;
        }
        // generate a start condition (SDA goes high -> low while SCL stays high)
        set_SDA(0);
        _NOP();
        set_SCL(0);
        // start communication
        return i2c_drvr_write_byte(addr_with_mode);
    }


    // Summary - 
    // retval (retval_t) - 
    retval_t i2c_drvr_end(void)
    {
        // generate a stop condition (SDA goes low -> high while SCL stays high)
        set_SDA(0);
        set_SCL(1);  // double check that SCL is deasserted as well
        _NOP();
        set_SDA(1);
        return GEN_PASS;
    }


    // Summary - 
    // param (uint8_t) byte_to_tx - 
    retval_t i2c_drvr_write_byte(uint8_t byte_to_tx)
    {
        int8_t i;
        uint8_t bit;
        retval_t retval;
        // send byte
        for (i = 7; i >= 0; i--) {
            bit = (byte_to_tx & 0x80) >> 7; // bring MSB to bit 0
            set_SCL(0);
            set_SDA(bit);
            set_SCL(1);  // clock it in
            byte_to_tx = byte_to_tx << 1; // shift byte
        }
        set_SCL(0);
        set_SDA(1); // deassert the data line
        retval = check_ack();
        if (retval == I2C_ACK) {
            return GEN_PASS;
        } else {
            return retval;
        }
    }


    // Summary - 
    // retval (retval_t) - 
    retval_t i2c_drvr_read_byte(uint8_t *byte_read_ptr, uint8_t ack)
    {
        int8_t i;
        uint8_t bit;
        uint8_t byte = 0;
        retval_t retval;
        // make sure SDA is de-asserted
        set_SDA(1);
        for (i = 7; i >= 0; i--) {
            set_SCL(1);
            bit = read_SDA();
            byte = byte | (bit << i);
            set_SCL(0);
        }
        write_ack(ack);
        return GEN_PASS;
    }


    // Summary - 
    // retval (retval_t) - 
    retval_t check_ack(void)
    {
        uint8_t bit;
        set_SCL(1);
        _NOP();
        // check ack/nack here
        bit = read_SDA();
        set_SCL(0);
        if (bit == 0) {
            return I2C_ACK;
        } else {
            return I2C_NACK;
        }
    }

        // Summary - 
    // retval (retval_t) - 
    void write_ack(uint8_t val)
    {
        if (val == ACK) {
            set_SDA(0);
        } else {
            set_SDA(1);
        }
        set_SCL(1);
        _NOP();
        set_SCL(0);
    }

#else // Non-bitbang version

    void toggle_scl();

    // Summary - 
    // retval (retval_t) - 
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
    // retval (retval_t) - 
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
    // retval (retval_t) - 
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
    // retval (retval_t) - 
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
    // retval (retval_t) - 
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


#endif  /* I2C_BITBANG */
