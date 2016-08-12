#ifndef I2C_H__
#define I2C_H__

// I2C constant definitions
#define I2C_MODE_READ  1
#define I2C_MODE_WRITE 0
#define I2C_RETRY_COUNT 3

// I2C return codes
#define I2C_NO_ERRORS 0
#define I2C_ERROR 1


typedef struct {
    uint8_t address;
    uint8_t mode;
} i2c_admin_t;

// function definitions


#endif
