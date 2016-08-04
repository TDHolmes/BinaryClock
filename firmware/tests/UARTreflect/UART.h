#ifndef UART_H__
#define UART_H__

#include <stdint.h>
#include "UART_drvr_2313a.h"

// error codes
#define UART_CMD_INVALID     0xFF
#define UART_CMD_WRONG_STATE 0xFE
// valid command
#define UART_CMD_VALID       0x42

#define UART_CMD_SET_LED       0x10
#define UART_CMD_CLEAR_LED     0x11
#define UART_CMD_CLEAR_ALL_LED 0x12
#define UART_CMD_SET_ALL_LED   0x13
#define UART_CMD_SET_TIME      0x14
#define UART_CMD_SET_COLOR     0x15
#define UART_CMD_CHANGE_STATE  0x16

#include <stdint.h>
#include "UART_drvr_2313a.h"

void UART_init(void);
void UART_transmit(uint8_t *data_to_tx_ptr, uint8_t len);
void UART_transmit_byte(uint8_t data_to_tx);
void UART_receive(uint8_t *receive_buffer_ptr, uint8_t len);
uint8_t UART_receive_byte(void);
uint8_t UART_receive_has_data(void);
void UART_reset(void);

#endif /* UART_H__ */
