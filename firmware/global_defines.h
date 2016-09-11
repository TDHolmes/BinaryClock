#ifndef GLOBAL_DEFINES_H__
#define GLOBAL_DEFINES_H__


//! definition of the types of return values possible
typedef enum {
    GEN_PASS = 0,  //!< Value to return if everything went well
    GEN_FAIL,      //!< General failure message
    BUFF_FAIL_OVERFLOW = 0x11,  //!< return if buffer overflow
    BUFF_FAIL_UNDERFLOW,   //!< return if buffer underflow
    BUFF_FAIL_FULL,        //!< return if buffer is already full
    UART_RX_EMPTY = 0x21,  //!< return if there is nothing to receive
    UART_FRAME_ERROR,      //!< return if UART framing error
    UART_DATA_OVERRUN_ERROR, //!< return if UART overrun error
    UART_PARITY_ERROR,     //!< return if UART parity error
    I2C_ACK = 0x30,    //!< return if the device acknowledges the message
    I2C_NACK,          //!< return is the device does not acknowledge the message
    I2C_ERROR,         //!< return if there is a general I2C error
    I2C_START_TIMEOUT, //!< return if the device does not respond in time
    I2C_INIT_TIMEOUT,  //!< return if the I2C module does not initialize in time
    I2C_CLK_LOW_BEFORE_START,   //!< return if the SCL line is low when it shouldn't be
    I2C_START_COND_NOT_DETECTED //!< return if the start condition was not detected
} retval_t;


//! definition of a custom boolean type
typedef enum {
    FALSE = 0, //!< False boolian value
    TRUE       //!< True boolian value
} bool_t;

#endif /* GLOBAL_DEFINES_H__ */
