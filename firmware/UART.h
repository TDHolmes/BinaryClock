#ifndef UART_H__
#define UART_H__

#include "UART_drvr_4313.h"
#include "global_defines.h"
#include <stdint.h>

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

#define LF 0x0A  //!< ascii value for line feed
#define CR 0x0D  //!< ascii value for carriage return


void UART_init(uint32_t baudrate);
void UART_transmit(uint8_t *data_to_tx_ptr, uint8_t len, bool_t blocking);
void UART_transmit_byte(uint8_t data_to_tx, bool_t blocking);
void UART_transmit_value(int32_t var_to_tx,  bool_t blocking, bool_t print_hex);
void UART_receive(uint8_t *receive_buffer_ptr, uint8_t len);
retval_t UART_receive_byte(uint8_t *data_to_rx_ptr, bool_t blocking);
bool_t   UART_receive_has_data(void);
uint8_t UART_receive_unread_items(void);
void UART_reset(void);

#endif /* UART_H__ */
