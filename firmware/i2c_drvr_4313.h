#ifndef I2C_DRVR_4313_H__
#define I2C_DRVR_4313_H__

#include "global_defines.h"
#include <stdint.h>

#define I2C_START_RETRY 5


// function definitions
retval_t i2c_drvr_init(void);
retval_t i2c_drvr_start(uint8_t byte_to_tx);
retval_t i2c_drvr_end(void);
retval_t i2c_drvr_write_byte(uint8_t byte_to_tx);
retval_t i2c_drvr_read_byte(uint8_t *byte_read_ptr);

void toggle_scl(void);

#endif /* I2C_DRVR_4313_H__ */