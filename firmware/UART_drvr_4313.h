#ifndef UART_DRVR_4313_H__
#define UART_DRVR_4313_H__

#include "hardware.h"
#include "global_defines.h"
#include <stdint.h>

/*! \file UART_drvr_4313.h
    \brief Low level driver definitions for UART communications on the ATtiny4313.
*/

#define UART_RX_BUFF_SIZE 16 //!< size of the UART receive buffer


typedef enum {
    BAUD_250000 = 1,
    BAUD_57600  = 8,
    BAUD_38400  = 12,
    BAUD_19200  = 25,
    BAUD_14400  = 34,
    BAUD_9600   = 51
} baudrate_t;


void UART_drvr_init(baudrate_t baudrate);
void UART_drvr_send_byte(uint8_t data, bool_t blocking);
retval_t UART_drvr_receive_byte(uint8_t *data_rcvd_ptr);
void UART_drvr_flush_buffer(void);
retval_t UART_drvr_check_errors(void);
void UART_drvr_clear_errors(void);
bool_t UART_drvr_receive_has_data(void);
uint8_t UART_drvr_unread_items(void);


#endif /* UART_DRVR_4313_H__ */
