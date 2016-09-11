#ifndef GLOBAL_DEFINES_H__
#define GLOBAL_DEFINES_H__

/*! \file global_defines.h
    \brief Definitions and types used in all files. Should be included in all project files.
*/


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


//! code if the UART command received is invalid
#define UART_CMD_INVALID     0xFF
//! code to send if the state is incorrect for the given command
#define UART_CMD_WRONG_STATE 0xFE
//! valid command & successful
#define UART_CMD_VALID       0x42

//! byte that starts each transaction
#define UART_CMD_START_BYTE  0xAA

//! command to set an LED to a certain color (must be in manual control mode)
#define UART_CMD_SET_LED       0x10
//! command to set an LED to a certain color (must be in manual control mode)
#define UART_CMD_CLEAR_LED     0x11
//! command to clear an led (must be in manual control mode)
#define UART_CMD_CLEAR_ALL_LED 0x12
//! command to clear all leds (must be in manual control mode)
#define UART_CMD_SET_ALL_LED   0x13
//! command to set all leds to a certain color (must be in manual control mode)
#define UART_CMD_SET_TIME      0x14
//! command to set the time (must be in time control mode)
#define UART_CMD_SET_COLOR     0x15
//! command to set the color during time mode (must be in time control mode)
#define UART_CMD_CHANGE_STATE  0x16
//! command to change states


//! definition of a custom boolean type
typedef enum {
    FALSE = 0, //!< False boolian value
    TRUE       //!< True boolian value
} bool_t;

#endif /* GLOBAL_DEFINES_H__ */
