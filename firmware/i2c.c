#include "i2c.h"
#include "i2c_drvr_4313.h"

// implement transmit byte, receive byte
typedef struct {
    uint8_t address;
    uint8_t mode;
} i2c_drvr_t;

void i2c_start(uint8_t address, uint8_t mode)
{

}

uint8_t i2c_write(uint8_t start_adr, uint8_t *data_to_write_ptr, uint8_t data_len)
{
    return I2C_NO_ERRORS;
}

uint8_t i2c_read(uint8_t start_adr, uint8_t *data_out_ptr, uint8_t data_len)
{
    return I2C_NO_ERRORS;
}

void i2c_end(uint8_t address)
{

}