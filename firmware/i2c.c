#include "i2c.h"
#include "i2c_drvr_4313.h"
#include "global_defines.h"
#include <stdint.h>

#define WRITE_MASK 0x00
#define READ_MASK  0x01


// private functions
static retval_t i2c_start(uint8_t address, uint8_t mode);
static void i2c_end(void);


// Summary - 
// retval (uint8_t) - 
retval_t i2c_init(void)
{
    return i2c_drvr_init();
}


// Summary - 
// param (uint8_t) address - 
// param (uint8_t) mode - 
// retval (uint8_t) - 
retval_t i2c_start(uint8_t address, uint8_t mode)
{
    uint8_t addr_byte_to_send;
    uint8_t i;
    retval_t retval = I2C_ERROR;

    // try to start communication
    if (mode == I2C_MODE_WRITE) {
        addr_byte_to_send = (address << 1) & ~(0x01);
    } else {
        addr_byte_to_send = (address << 1) | 0x01;
    }
    // send the initial address in the mode
    retval = i2c_drvr_start(addr_byte_to_send);
    return retval;
}


retval_t i2c_write_byte(uint8_t chip_addr, uint8_t reg_addr, uint8_t data)
{
    volatile uint8_t i;
    // start communications with the chip
    retval_t retval = i2c_start(chip_addr, I2C_MODE_WRITE);
    // check for device ack
    if (retval != GEN_PASS) {
        return retval;
    }
    // write the address byte
    retval = i2c_drvr_write_byte(reg_addr);
    if (retval != GEN_PASS) {
        i2c_end(); // try to end communication
        return retval;
    }
    retval = i2c_drvr_write_byte(data);
    i2c_end();    // end communication
    return retval;
}


// Summary - 
// param (uint8_t) start_adr - 
// param (uint8_t *) data_to_write_ptr - 
// param (uint8_t) data_len - 
// retval (uint8_t) - 
retval_t i2c_write(uint8_t chip_addr, uint8_t start_adr, uint8_t *data_to_write_ptr, uint8_t data_len)
{
    // start communications with the chip
    retval_t retval = i2c_start(chip_addr, I2C_MODE_WRITE);
    // check for device ack
    if (retval != GEN_PASS) {
        return retval;
    }
    // write the address byte
    retval = i2c_drvr_write_byte(start_adr);
    if (retval != GEN_PASS) {
        // data write failure
        i2c_end(); // try to end communication
        return retval;
    }
    for ( ; data_len != 0; data_len--) {
        retval = i2c_drvr_write_byte(*data_to_write_ptr);  // send a byte
        if (retval != GEN_PASS) {
            // data write failure
            i2c_end(); // try to end communication
            return retval;
        }
        data_to_write_ptr += 1;  // go to next byte
    }
    i2c_end();
    return GEN_PASS;
}


// Summary - 
// param (uint8_t) start_adr - 
// param (uint8_t *) data_out_ptr - 
// param (uint8_t) data_len - 
// retval (uint8_t) - 
retval_t i2c_read_byte(uint8_t chip_addr, uint8_t start_adr, uint8_t *data_out_ptr)
{
    // start communications with the chip in write mode to change the address
    retval_t retval = i2c_start(chip_addr, I2C_MODE_WRITE);
    // check for device ack
    if (retval != GEN_PASS) {
        i2c_end(); // try to end communication
        return retval;
    }
    // write the address byte
    retval = i2c_drvr_write_byte(start_adr);
    if (retval != GEN_PASS) {
        i2c_end(); // try to end communication
        return retval;
    }
    i2c_end();
    // RE-start communications with the chip in write mode to change the address
    retval = i2c_start(chip_addr, I2C_MODE_READ);
    if (retval != GEN_PASS) {
        i2c_end(); // try to end communication
        return retval;
    }
    // read data
    retval = i2c_drvr_read_byte(data_out_ptr, NACK); // read a byte & NACK
    // check for errors
    if (retval != GEN_PASS) {
        i2c_end(); // try to end communication
        return retval;
    } else {
        i2c_end();
        return GEN_PASS;
    }
}


// Summary - 
// param (uint8_t) start_adr - 
// param (uint8_t *) data_out_ptr - 
// param (uint8_t) data_len - 
// retval (uint8_t) - 
retval_t i2c_read(uint8_t chip_addr, uint8_t start_adr, uint8_t *data_out_ptr, uint8_t data_len)
{
    uint8_t i;
    // start communications with the chip in write mode to change the address
    retval_t retval = i2c_start(chip_addr, I2C_MODE_WRITE);
    // check for device ack
    if (retval != GEN_PASS) {
        i2c_end(); // try to end communication
        return retval;
    }
    // write the address byte
    retval = i2c_drvr_write_byte(start_adr);
    if (retval != GEN_PASS) {
        i2c_end(); // try to end communication
        return retval;
    }
    i2c_end();
    // RE-start communications with the chip in write mode to change the address
    retval = i2c_start(chip_addr, I2C_MODE_READ);
    if (retval != GEN_PASS) {
        i2c_end(); // try to end communication
        return retval;
    }
    for (i = 0; i < data_len; i++) {
        if (i != (data_len - 1)) {
            retval = i2c_drvr_read_byte(data_out_ptr, ACK); // read a byte & ack
        } else {
            retval = i2c_drvr_read_byte(data_out_ptr, NACK); // read a byte & NACK
        }
        // check for errors
        if (retval != GEN_PASS) {
            i2c_end(); // try to end communication
            return retval;
        }
        data_out_ptr += 1;
    }
    i2c_end();
    return GEN_PASS;
}


// Summary - 
void i2c_end(void)
{
    i2c_drvr_end();
}
