#ifndef UART_DRVR_2313A_H__
#define UART_DRVR_2313A_H__

#include "hardware.h"

#define UART_BAUDRATE (250000)
#define UART_PRESCALE (((F_OSC >> 4UL) / UART_BAUDRATE) - 1UL)
#define UART_RX_BUFF_SIZE 16

#define UART_RX_EMPTY 1

#endif /* UART_DRVR_2313A_H__ */
