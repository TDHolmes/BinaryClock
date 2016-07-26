#include <stdint.h>
#include "avr/io.h"
#include "ring_bufffer.h"
#include <stdint.h>
#include "UART_drvr_2313a.h"

uint8_t UART_rx_buff[UART_RX_BUFF_SIZE];

ring_buff_t UART_rx_buff;
ring_buff_t *UART_rx_buff_ptr;

void UART_drvr_init(void)
{
    // setup UART RX ring buffer
    ring_buff_init(UART_rx_buff, UART_rx_buff, UART_RX_BUFF_SIZE);
    UART_rx_buff_ptr = &UART_rx_buff;
    // Set baud rate
    UBRRH = (uint8_t)(UART_PRESCALE >> 8);
    UBRRL = (uint8_t)UART_PRESCALE;
    // Enable receiving interrupts
    UCSRB = UCSRB | (1 << RXCIE);
    // Set frame format: 8data, 2stop bit
    UCSRC = (0 << USBS) | (3 << UCSZ0);
    // Enable receiver and transmitter
    UCSRB = (1 << RXEN) | (1 << TXEN);
}


void UART_drvr_send_byte(uint8_t data)
{
    // Wait for empty transmit buffer
    while (!(UCSRA & (1 << UDRE)));
    // Put data into buffer, sends the data
    UDR = data;
}


uint8_t UART_drvr_receive_byte(uint8_t *data_rcvd_ptr)
{
    if(ring_buff_has_data(UART_rx_buff_ptr)) {
        return ring_buff_pop(UART_rx_buff_ptr, &data_rcvd);
    }
    else {
        return UART_RX_EMPTY;
    }
}


void UART_drvr_flush_buffer(void)
{
    uint8_t dummy;
    while (UCSRA & (1 << RXC)) {
        dummy = UDR;
    }
}


uint8_t UART_drvr_check_errors(void)
{
    if (UCSRA & (1 << UPE | 1 << DOR | 1 << UPE)) {
        return FAIL;
    } else {
        return PASS;
    }
}


void UART_drvr_clear_errors(void)
{
    UCSRA = UCSRA & ~(1 << UPE | 1 << DOR | 1 << UPE);
}


ISR(USART0_RX_vect)
{
    // Wait for data to be received
    while (!(UCSRA & (1 << RXC)));
    // Get and push received data
    ring_buff_push(UART_rx_buff_ptr, UDR);
}