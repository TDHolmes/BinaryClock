#include "i2c.h"
#include "i2c_drvr_4313.h"

#define WRITE_MASK 0x01
#define READ_MASK  0x00

i2c_admin_t i2c_admin;
i2c_admin_t i2c_admin_ptr;

// Summary - 
// retval (uint8_t) - 
uint8_t i2c_init(void)
{
    i2c_admin.address = 0;
    return i2c_drvr_init();
}


// Summary - 
// param (uint8_t) address - 
// param (uint8_t) mode - 
// retval (uint8_t) - 
uint8_t i2c_start(uint8_t address, uint8_t mode)
{
    uint8_t addr_byte_to_send;
    uint8_t i;
    uint8_t retval = FAIL;

    // try to start communication
    for (i = I2C_RETRY_COUNT; i != 0; i--) {
        if (mode == I2C_MODE_WRITE) {
            addr_byte_to_send = (address << 1) | WRITE_MASK;
        } else {
            addr_byte_to_send = (address << 1) | READ_MASK;
        }
        // send the initial address in the mode
        if (i2c_drvr_start(addr_byte_to_send) == I2C_ACK) {
            i2c_admin.address = address;
            i2c_admin.mode = mode;
            i2c_admin_ptr = &i2c_admin;
            retval = PASS;
            break;
        }
    }

    return retval;
}


// Summary - 
// param (uint8_t) start_adr - 
// param (uint8_t *) data_to_write_ptr - 
// param (uint8_t) data_len - 
// retval (uint8_t) - 
uint8_t i2c_write(uint8_t start_adr, uint8_t *data_to_write_ptr, uint8_t data_len)
{
    uint8_t retval;
    // check for the correct mode
    if (i2c_admin_ptr->mode != I2C_MODE_WRITE) {
        return I2C_ERROR;
    }
    // write the address byte
    i2c_drvr_write_byte(start_adr);
    for ( ; data_len != 0; i--) {
        retval = i2c_drvr_write_byte(*data_to_write_ptr);  // send a byte
        if (retval != PASS) {
            // data write failure
            return retval;
        }
        data_to_write_ptr += 1;  // go to next byte
    }
    return I2C_NO_ERRORS;
}


// Summary - 
// param (uint8_t) start_adr - 
// param (uint8_t *) data_out_ptr - 
// param (uint8_t) data_len - 
// retval (uint8_t) - 
uint8_t i2c_read(uint8_t start_adr, uint8_t *data_out_ptr, uint8_t data_len)
{
    // check for the correct mode
    if (i2c_admin_ptr->mode != I2C_MODE_READ) {
        return I2C_ERROR;
    }
    return I2C_NO_ERRORS;
}


// Summary - 
// param (uint8_t) address - 
void i2c_end(uint8_t address)
{
    i2c_admin.address = 0;
}
