#ifndef GLOBAL_DEFINES_H__
#define GLOBAL_DEFINES_H__


// error / pass
typedef enum {
    GEN_PASS = 0,
    GEN_FAIL,
    BUFF_FAIL_OVERFLOW = 0x11,
    BUFF_FAIL_UNDERFLOW,
    BUFF_FAIL_FULL,
    UART_RX_EMPTY = 0x21,
    UART_FRAME_ERROR,
    UART_DATA_OVERRUN_ERROR,
    UART_PARITY_ERROR,
    I2C_ACK = 0x30,
    I2C_NACK,          // 0x31
    I2C_ERROR,         // 0x32
    I2C_START_TIMEOUT,    // 0x33
    I2C_INIT_TIMEOUT,     // 0x34
    I2C_CLK_LOW_BEFORE_START,   // 0x35
    I2C_START_COND_NOT_DETECTED // 0x36
} retval_t;

typedef enum {
    FALSE = 0,
    TRUE
} bool_t;

#endif /* GLOBAL_DEFINES_H__ */
