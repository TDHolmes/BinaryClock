#include "UART_drvr_4313.h"
#include "ports.h"
#include "hardware.h"
#include "ring_buffer.h"
#include "global_defines.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

/*! \file UART_drvr_4313.c
    \brief Low level driver functions for UART communications on the ATtiny4313.
*/

uint8_t UART_rx_buff_arr[UART_RX_BUFF_SIZE]; //!< buffer for the UART received ring buffer

ring_buffer_t UART_rx_buff;      //!< UART RX ring buffer admin structure.
ring_buffer_t *UART_rx_buff_ptr; //!< UART RX ring buffer admin struct pointer.

uint8_t byte_received = 0; //!< temp variable for the ISR to store the value received into.


static inline bool_t UART_drvr_ready(void);


/*!
 * Initializes the UART hardware to the given baudrate.
 * 
 * @param[in] baudrate (uint32_t): baudrate to set up the UART driver to.
 */
void UART_drvr_init(baudrate_t baudrate)
{
    uint16_t UART_prescale;
    UART_rx_buff_ptr = &UART_rx_buff;
    // setup UART RX ring buffer
    ring_buff_init(UART_rx_buff_ptr, UART_rx_buff_arr, UART_RX_BUFF_SIZE);
    // Set baud rate
    UART_prescale = baudrate;
    UBRRH = (uint8_t)(UART_prescale >> 8);
    UBRRL = (uint8_t)UART_prescale;
    // Set frame format: 8data, 1stop bit
    UCSRC = (0b011 << UCSZ0);
    // Enable receiver and transmitter & Enable receiving interrupts
    UCSRB = (1 << RXEN) | (1 << TXEN) | (1 << RXCIE);
}


/*!
 * Sends a byte via UART.
 * 
 * @param[in] data (uint8_t): data to send
 * @param[in] blocking (bool_t): whether or not to wait for the transmission
 *      to end
 */
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


/*!
 * Receives a byte from the ring buffer.
 * 
 * @param[in] data_rcvd_ptr (uint8_t *): location to store the data received.
 * @param[out] retval (retval_t): returns the success or failure of the action.
 */
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


/*!
 * Receives a byte from the ring buffer, but don't pop it off
 * 
 * @param[in] data_rcvd_ptr (uint8_t *): location to store the data received.
 * @param[out] retval (retval_t): returns the success or failure of the action.
 */
retval_t UART_drvr_peak_byte(uint8_t *data_rcvd_ptr)
{
    retval_t retval;
    if(ring_buff_has_data(UART_rx_buff_ptr) > 0) {
        cli();
        retval = ring_buff_peak(UART_rx_buff_ptr, data_rcvd_ptr);
        sei();
        return retval;
    }
    else {
        return UART_RX_EMPTY;
    }
}


/*!
 * Flushes the current data from the ring buffer and from the hardware UART module
 */
void UART_drvr_flush_buffer(void)
{
    uint8_t dummy;
    while (UCSRA & (1 << RXC)) {
        dummy = UDR;
    }
    ring_buff_reset(UART_rx_buff_ptr);
}


/*!
 * Checks for UART hardware errors:
 *      - Framing errors
 *      - overrun errors
 *      - parity errors
 *
 * @param[out] retval (retval_t): returns the success or failure of the action.
 */
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


/*!
 * Clears UART hardware errors.
 */
void UART_drvr_clear_errors(void)
{
    UCSRA = UCSRA & (~(1 << UPE | 1 << DOR | 1 << FE));
}


/*!
 * Returns true if there is data received currently.
 * 
 * @param[out] has_data (bool_t): returns true if the UART driver has data.
 */
bool_t UART_drvr_receive_has_data(void)
{
    return ring_buff_has_data(UART_rx_buff_ptr);
}


/*!
 * Returns the number of bytes currently received by the UART module.
 * 
 * @param[out] unread_items (uint8_t): the number of bytes received currently.
 */
uint8_t UART_drvr_unread_items(void)
{
    return ring_buff_unread_items(UART_rx_buff_ptr);
}


/*!
 * Returns true if the UART module isn't busy.
 * 
 * @param[out] UART_ready (bool_t): returns true if the UART driver isn't busy.
 */
static inline bool_t UART_drvr_ready(void)
{
    if (!(UCSRA & (1 << UDRE))) {
        return FALSE;
    } else {
        return TRUE;
    }
}


/*!
 * Receives a byte and psuhes it onto the ring buffer.
 */
ISR(USART0_RX_vect)
{
    cli();
    byte_received = UDR;
    ring_buff_push(UART_rx_buff_ptr, byte_received);
    sei();
}
