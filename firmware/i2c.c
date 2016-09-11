#include "i2c.h"
#include "i2c_drvr_4313.h"
#include "global_defines.h"
#include <stdint.h>

#define WRITE_MASK 0x00  //!< Mask for writing to a chip.
#define READ_MASK  0x01  //!< Mask for reading from a chip.


// private functions
static retval_t i2c_start(uint8_t address, uint8_t mode);
static void i2c_end(void);


/*!
 * Initializes the I2C driver.
 * 
 * @param[out] retval (retval_t): Returns the initialization status of the I2C driver.
 */
retval_t i2c_init(void)
{
    return i2c_drvr_init();
}


/*!
 * Starts communication with a chip in the given mode. Only local use!
 * 
 * @param[in]  address (uint8_t): Address of the chip to communicate with.
 * @param[in]  mode (uint8_t): Read/write mode to start communication
 * @param[out] retval (retval_t): Returns the success or failure of starting
 *      communication.
 */
retval_t i2c_start(uint8_t address, uint8_t mode)
{
    uint8_t addr_byte_to_send;
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


/*!
 * Writes a byte to a chip. Handles starting and stopping of communication.
 * 
 * @param[in]  chip_addr (uint8_t): The chip's address without the read/write bit set.
 * @param[in]  reg_addr (uint8_t): Address of the register you want to write to.
 * @param[in]  data (uint8_t): Data you want to write.
 * @param[out] retval (retval_t): Returns the success or failure of starting communication
 *      & sending the byte.
 */
retval_t i2c_write_byte(uint8_t chip_addr, uint8_t reg_addr, uint8_t data)
{
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


/*!
 * Starts communication with a chip and writes N bytes to the chip. Handles starting
 * and ending communication as well.
 * 
 * @param[in] chip_addr (uint8_t): The address of the chip to be written to.
 * @param[in] start_adr (uint8_t): The starting register of the write.
 * @param[in] data_to_write_ptr (uint8_t *): The data to be written array.
 * @param[in] data_len (uint8_t): How many bytes to be written.
 * @param[out] retval (retval_t): Returns the success or failure of starting
 *      communication and writing to the device.
 */
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


/*!
 * Reads a byte from a chip. Handles starting and stopping of communication.
 * 
 * @param[in] chip_addr (uint8_t): The address of the chip to be read from.
 * @param[in] start_adr (uint8_t): The starting register of the read.
 * @param[in] data_out_ptr (uint8_t *): Location to store the read value.
 * @param[out] retval (retval_t): Returns the success or failure of starting communication
 *      & reading the byte.
 */
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


/*!
 * Reads N bytes from a chip. Handles starting and stopping of communication.
 * 
 * @param[in] chip_addr (uint8_t): The address of the chip to be read from.
 * @param[in] start_adr (uint8_t): The starting register of the read.
 * @param[in] data_out_ptr (uint8_t *): Location to store the read values.
 * @param[in] data_len (uint8_t): amount of bytes to read.
 * @param[out] retval (retval_t): Returns the success or failure of starting communication
 *      & reading the byte.
 */
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


/*!
 * Ends communication with a chip in the given mode. Only local use!
 */
void i2c_end(void)
{
    i2c_drvr_end();
}
