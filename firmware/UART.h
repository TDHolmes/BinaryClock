#ifndef UART_H__
#define UART_H__

#include "UART_drvr_4313.h"
#include "global_defines.h"
#include <stdint.h>

/*! \file UART.h
    \brief High level definitions for UART communications.
*/


#define LF 0x0A  //!< ascii value for line feed
#define CR 0x0D  //!< ascii value for carriage return


void UART_init(void);
void UART_transmit(uint8_t *data_to_tx_ptr, uint8_t len, bool_t blocking);
void UART_transmit_byte(uint8_t data_to_tx, bool_t blocking);
void UART_transmit_value(int32_t var_to_tx,  bool_t blocking, bool_t print_hex);
void UART_receive(uint8_t *receive_buffer_ptr, uint8_t len);
retval_t UART_receive_byte(uint8_t *data_to_rx_ptr, bool_t blocking);
bool_t   UART_receive_has_data(void);
uint8_t UART_receive_unread_items(void);
void UART_reset(void);

#endif /* UART_H__ */
