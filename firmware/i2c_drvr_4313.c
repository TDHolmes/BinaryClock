#include "i2c_drvr_4313.h"
#include <avr/io.h>


uint8_t i2c_drvr_init(void)
{
    // USICR USI Control register
    USICR = 0b00110010;
}

uint8_t i2c_drvr_start(uint8_t byte_to_tx)
{
    // generate a start condition (SDA goes high -> low while SCL stays high)
    i2c_drvr_write_byte(byte_to_tx);
    // check ack
    toggle_scl(void);
    // check ack/nack here
    toggle_scl(void);
    // return ack/nack
}

uint8_t i2c_drvr_end(void)
{
    // generate a stop condition (SDA goes low -> high while SCL stays high)
}

void i2c_drvr_write_byte(uint8_t byte_to_tx)
{
    uint8_t i;
    // send byte
    USIDR = byte_to_tx;
    for (i = 7; i != 0; i--) {
        toggle_scl(void);
        toggle_scl(void);
    }
}

uint8_t i2c_drvr_read_byte(void)
{
    uint8_t i;
    for (i = 7; i != 0; i--) {
        toggle_scl(void);
        toggle_scl(void);
    }
    // read from i2c buffer
    return USIBR;
}

inline void toggle_scl(void)
{
    USICR = USICR | 0x01;
}


