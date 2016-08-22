#include "UART_drvr_4313.h"
#include "global_defines.h"
#include <avr/interrupt.h>
#include <stdint.h>


// Summary - 
void UART_init(uint32_t baudrate)
{
    UART_drvr_init(baudrate);
}

// Summary - 
// param (uint8_t *) data_to_tx_ptr - 
// param (uint8_t) len - 
// param (bool_t) blocking - 
void UART_transmit(uint8_t *data_to_tx_ptr, uint8_t len, bool_t blocking)
{
    uint8_t bytes_left = len;
    uint8_t *current_data_to_tx_ptr = data_to_tx_ptr;
    while(bytes_left != 0) {
        UART_drvr_send_byte(*current_data_to_tx_ptr, blocking);
        current_data_to_tx_ptr += 1;
        bytes_left -= 1;
    }
}


// Summary - 
// param (uint8_t) data_to_tx - 
// param (bool_t) blocking - 
void UART_transmit_byte(uint8_t data_to_tx, bool_t blocking)
{
    UART_drvr_send_byte(data_to_tx, blocking);
}


// Summary - 
// param (int32_t) var_to_tx - 
void UART_transmit_value(int32_t var_to_tx,  bool_t blocking, bool_t print_hex)
{
    uint8_t data_to_tx[10];
    uint8_t temp_val;
    int8_t i;
    // initialize transmit array
    for (i = 9; i >= 0; i--) {
        data_to_tx[i] = 0;
    }
    // handle negative variables
    if (var_to_tx < 0) {
        UART_transmit_byte('-', blocking);
        var_to_tx = (-1) * var_to_tx;
    }
    if (print_hex == TRUE) {
        UART_transmit_byte('0', blocking);
        UART_transmit_byte('x', blocking);
    }
    // fill transmit array
    i = 0;
    while (var_to_tx >= 10) {
        if (print_hex == FALSE) {
            data_to_tx[i] = (var_to_tx % 10) + 0x30;  // ascii value of number
            var_to_tx = var_to_tx / 10;
        } else {
            // print hex
            temp_val = (var_to_tx % 16);
            if (temp_val <= 9) {
                data_to_tx[i] = temp_val + 0x30;
            } else {
                data_to_tx[i] = temp_val + 0x45;  // ("A" offset) - 10 = 0x45
            }
            var_to_tx = var_to_tx >> 4;  // divide by 16
        }
        if (i < 10) {
            i++;
        }
    }
    // add last character
    if (print_hex == TRUE) {
        if (var_to_tx <= 9) {
            data_to_tx[i] = var_to_tx + 0x30;
        } else {
            data_to_tx[i] = var_to_tx + 0x45;  // ("A" offset) - 10 = 0x45
        }
    } else {
        data_to_tx[i] = var_to_tx + 0x30;
    }
    // transmit data
    for (i = 9; i >= 0; i--) {
        if (data_to_tx[i] != 0) {
            UART_transmit_byte(data_to_tx[i], blocking);
        }
    }
}


// Summary - 
// param (uint8_t *) receive_buffer_ptr - 
// param (uint8_t) len - 
void UART_receive(uint8_t *receive_buffer_ptr, uint8_t len)
{
    uint8_t bytes_left = len;
    uint8_t *data_to_rx_ptr = receive_buffer_ptr;
    retval_t retval;
    while(bytes_left != 0) {
        retval = UART_drvr_receive_byte(data_to_rx_ptr);
        if (retval == GEN_PASS) {
            data_to_rx_ptr += 1;
            bytes_left -= 1;
        }
    }
}


// Summary - 
// retval (uint8_t) - 
retval_t UART_receive_byte(uint8_t *data_to_rx_ptr, bool_t blocking)
{
    retval_t retval = GEN_FAIL;
    do {
        retval = UART_drvr_receive_byte(data_to_rx_ptr);
    } while (blocking == TRUE && retval != GEN_PASS);
    return retval;
}


// Summary - 
// retval (uint8_t) - 
bool_t UART_receive_has_data(void)
{
    return UART_drvr_receive_has_data();
}


// Summary - 
void UART_reset(void)
{
    cli();
    UART_drvr_flush_buffer();
    UART_drvr_clear_errors();
    sei();
}

