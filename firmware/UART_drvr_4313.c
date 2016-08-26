#include "UART_drvr_4313.h"
#include "ports.h"
#include "hardware.h"
#include "ring_buffer.h"
#include "global_defines.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

uint8_t UART_rx_buff_arr[UART_RX_BUFF_SIZE];

ring_buffer_t UART_rx_buff;
ring_buffer_t *UART_rx_buff_ptr;

uint8_t byte_received = 0;

static inline bool_t UART_drvr_ready(void);

// Summary - 
void UART_drvr_init(uint32_t baudrate)
{
    uint16_t UART_prescale;
    UART_rx_buff_ptr = &UART_rx_buff;
    // setup UART RX ring buffer
    ring_buff_init(UART_rx_buff_ptr, UART_rx_buff_arr, UART_RX_BUFF_SIZE);
    // Set baud rate
    UART_prescale = (((F_OSC >> 4UL) / baudrate) - 1UL);
    UBRRH = (uint8_t)(UART_prescale >> 8);
    UBRRL = (uint8_t)UART_prescale;
    // Set frame format: 8data, 1stop bit
    UCSRC = (0b011 << UCSZ0);
    // Enable receiver and transmitter & Enable receiving interrupts
    UCSRB = (1 << RXEN) | (1 << TXEN) | (1 << RXCIE);
}


// Summary - 
// param (uint8_t) data - 
void UART_drvr_send_byte(uint8_t data, bool_t blocking)
{
    // Wait for empty transmit buffer
    while (UART_drvr_ready() == FALSE);
    // Put data into buffer, sends the data
    UDR = data;
    if (blocking == TRUE) {
        // wait for the byte to finish sending
        while (UART_drvr_ready() == FALSE);
    }
}


// Summary - 
// param (uint8_t *) data_rcvd_ptr - 
// retval (uint8_t) - 
retval_t UART_drvr_receive_byte(uint8_t *data_rcvd_ptr)
{
    retval_t retval;
    if(ring_buff_has_data(UART_rx_buff_ptr) > 0) {
        cli();
        retval = ring_buff_pop(UART_rx_buff_ptr, data_rcvd_ptr);
        sei();
        return retval;
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
    ring_buff_reset(UART_rx_buff_ptr);
}


// Summary - 
// retval (uint8_t) - 
retval_t UART_drvr_check_errors(void)
{
    if (UCSRA & (1 << FE)) {
        return UART_FRAME_ERROR;
    } else if (UCSRA & (1 << DOR)) {
        return UART_DATA_OVERRUN_ERROR;
    } else if (UCSRA & (1 << UPE)) {
        return UART_PARITY_ERROR;
    } else {
        return GEN_PASS;
    }
}


// Summary - 
void UART_drvr_clear_errors(void)
{
    UCSRA = UCSRA & (~(1 << UPE | 1 << DOR | 1 << FE));
}


// Summary - 
// retval (uint8_t) - 
bool_t UART_drvr_receive_has_data(void)
{
    return ring_buff_has_data(UART_rx_buff_ptr);
}


// Summary - 
// retval (uint8_t) - 
uint8_t UART_drvr_unread_items(void)
{
    return ring_buff_unread_items(UART_rx_buff_ptr);
}


static inline bool_t UART_drvr_ready(void)
{
    if (!(UCSRA & (1 << UDRE))) {
        return FALSE;
    } else {
        return TRUE;
    }
}


// Summary - 
ISR(USART0_RX_vect)
{
    cli();
    byte_received = UDR;
    ring_buff_push(UART_rx_buff_ptr, byte_received);
    sei();
}
