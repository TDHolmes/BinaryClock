#ifndef I2C_H__
#define I2C_H__

#include "hardware.h"
#include "global_defines.h"
#include <stdint.h>

// I2C constant definitions
#define I2C_MODE_READ  1 //!< I2C read mode definition
#define I2C_MODE_WRITE 0 //!< I2C write mode definition

#define I2C_RETRY_COUNT 3 //!< number of times to retry

#define ACK  0 //!< defines an acknowledge condition (low)
#define NACK 1 //!< defines a neg-ack condition (high)

// function definitions 
retval_t i2c_init(void);
retval_t i2c_write_byte(uint8_t chip_addr, uint8_t reg_addr, uint8_t data);
retval_t i2c_write(uint8_t chip_addr, uint8_t start_adr, uint8_t *data_to_write_ptr, uint8_t data_len);
retval_t i2c_read_byte(uint8_t chip_addr, uint8_t start_adr, uint8_t *data_out_ptr);
retval_t i2c_read(uint8_t chip_addr, uint8_t start_adr, uint8_t *data_out_ptr, uint8_t data_len);

#endif
