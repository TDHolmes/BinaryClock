#ifndef UART_H__
#define UART_H__

#include "UART_drvr_4313.h"
#include "global_defines.h"
#include <stdint.h>

// error codes
#define UART_CMD_INVALID     0xFF
#define UART_CMD_WRONG_STATE 0xFE
// valid command
#define UART_CMD_VALID       0x42

#define UART_CMD_START_BYTE  0xAA

#define UART_CMD_SET_LED       0x10
#define UART_CMD_CLEAR_LED     0x11
#define UART_CMD_CLEAR_ALL_LED 0x12
#define UART_CMD_SET_ALL_LED   0x13
#define UART_CMD_SET_TIME      0x14
#define UART_CMD_SET_COLOR     0x15
#define UART_CMD_CHANGE_STATE  0x16

#define LF 0x0A
#define CR 0x0D


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
