#include <stdint.h>
#include "UART_drvr_2313a.h"

void UART_init(void)
{
    UART_drvr_init()
}

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


void UART_transmit_byte(uint8_t data_to_tx)
{
    UART_drvr_send_byte(data_to_tx);
}


void UART_receive(uint8_t *receive_buffer_ptr, uint8_t len)
{
    uint8_t bytes_left = len;
    uint8_t *current_data_to_rx_ptr = receive_buffer_ptr;
    uint8_t retval;
    while(bytes_left != 0) {
        retval = UART_drvr_receive_byte(current_data_to_rx_ptr);
        if (retval == PASS) {
            current_data_to_rx_ptr += 1;
            bytes_left -= 1;
        }
    }
}


uint8_t UART_receive_byte(void)
{
    uint8_t bytes_left = len;
    uint8_t *current_data_to_rx_ptr;
    uint8_t retval = FAIL;
    while(retval != PASS) {
        retval = UART_drvr_receive_byte(current_data_to_rx_ptr);
    }
    return *current_data_to_rx_ptr;
}