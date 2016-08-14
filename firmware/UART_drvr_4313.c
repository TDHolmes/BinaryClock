#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "ring_buffer.h"
#include "UART_drvr_4313.h"

uint8_t UART_rx_buff_arr[UART_RX_BUFF_SIZE];

ring_buffer_t UART_rx_buff;
ring_buffer_t *UART_rx_buff_ptr;

// Summary - 
void UART_drvr_init(void)
{
    UART_rx_buff_ptr = &UART_rx_buff;
    // setup UART RX ring buffer
    ring_buff_init(UART_rx_buff_ptr, UART_rx_buff_arr, UART_RX_BUFF_SIZE);
    // Set baud rate
    UBRRH = (uint8_t)(UART_PRESCALE >> 8);
    UBRRL = (uint8_t)UART_PRESCALE;
    // Set frame format: 8data, 2stop bit
    UCSRC = (0 << USBS) | (3 << UCSZ0);
    // Enable receiver and transmitter
    UCSRB = (1 << RXEN) | (1 << TXEN);
    // Enable receiving interrupts
    UCSRB = UCSRB | (1 << RXCIE);
}


// Summary - 
// param (uint8_t) data - 
void UART_drvr_send_byte(uint8_t data)
{
    // Wait for empty transmit buffer
    while (!(UCSRA & (1 << UDRE)));
    // Put data into buffer, sends the data
    UDR = data;
}


// Summary - 
// param (uint8_t *) data_rcvd_ptr - 
// retval (uint8_t) - 
uint8_t UART_drvr_receive_byte(uint8_t *data_rcvd_ptr)
{
    if(ring_buff_has_data(UART_rx_buff_ptr)) {
        return ring_buff_pop(UART_rx_buff_ptr, data_rcvd_ptr);
    }
    else {
        return UART_RX_EMPTY;
    }
}


// Summary - 
void UART_drvr_flush_buffer(void)
{
    uint8_t dummy;
    while (UCSRA & (1 << RXC)) {
        dummy = UDR;
    }
}


// Summary - 
// retval (uint8_t) - 
uint8_t UART_drvr_check_errors(void)
{
    if (UCSRA & (1 << UPE | 1 << DOR | 1 << UPE)) {
        return FAIL;
    } else {
        return PASS;
    }
}


// Summary - 
void UART_drvr_clear_errors(void)
{
    UCSRA = UCSRA & ~(1 << UPE | 1 << DOR | 1 << UPE);
}


// Summary - 
// retval (uint8_t) - 
uint8_t UART_drvr_receive_has_data(void)
{
    return ring_buff_has_data(UART_rx_buff_ptr);
}


// Summary - 
ISR(USART0_RX_vect)
{
    // Wait for data to be received
    while (!(UCSRA & (1 << RXC)));
    // Get and push received data
    ring_buff_push(UART_rx_buff_ptr, UDR);
}
