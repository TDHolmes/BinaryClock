#include <stdint.h>
#include "UART_drvr_2313a.h"

// Summary - 
void UART_init(void)
{
    UART_drvr_init();
}

// Summary - 
// param (uint8_t *) data_to_tx_ptr - 
// param (uint8_t) len - 
void UART_transmit(uint8_t *data_to_tx_ptr, uint8_t len)
{
    uint8_t bytes_left = len;
    uint8_t *current_data_to_tx_ptr = data_to_tx_ptr;
    while(bytes_left != 0) {
        UART_drvr_send_byte(*current_data_to_tx_ptr);
        current_data_to_tx_ptr += 1;
        bytes_left -= 1;
    }
}


// Summary - 
// param (uint8_t) data_to_tx - 
void UART_transmit_byte(uint8_t data_to_tx)
{
    UART_drvr_send_byte(data_to_tx);
}


// Summary - 
// param (int32_t) var_to_tx - 
void UART_transmit_value(int32_t var_to_tx)
{
    uint8_t data_to_tx[10];
    uint8_t i;
    // initialize transmit array
    for (i = 9; i >= 0; i--) {
        data_to_tx[i] = 0;
    }
    i = 0;
    // handle negative variables
    if (var_to_tx < 0) {
        UART_transmit_byte('-');
        var_to_tx = (-1) * var_to_tx;
    }
    // fill transmit array
    while (var_to_tx >= 10) {
        data_to_tx[i] = var_to_tx % 10 + 0x30;  // ascii value of number
        var_to_tx = var_to_tx / 10;
    }
    // transmit data
    for (i = 9; i >= 0; i--) {
        if (data_to_tx[i] != 0) {
            UART_transmit_byte(data_to_tx[i]);
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
    uint8_t retval;
    while(bytes_left != 0) {
        retval = UART_drvr_receive_byte(data_to_rx_ptr);
        if (retval == PASS) {
            data_to_rx_ptr += 1;
            bytes_left -= 1;
        }
    }
}


// Summary - 
// retval (uint8_t) - 
uint8_t UART_receive_byte(void)
{
    uint8_t *data_to_rx_ptr = 0;
    uint8_t retval = FAIL;
    while(retval != PASS) {
        retval = UART_drvr_receive_byte(data_to_rx_ptr);
    }
    return *data_to_rx_ptr;
}


// Summary - 
// retval (uint8_t) - 
uint8_t UART_receive_has_data(void)
{
    return UART_drvr_receive_has_data();
}


// Summary - 
void UART_reset(void)
{
    UART_drvr_flush_buffer();
    UART_drvr_clear_errors();
}

