#include <avr/io.h>
#include <stdint.h>
#include "UART.h"

int main()
{
    uint8_t byte_rx;
    UART_init();
    while (1) {
        if (UART_receive_has_data()) {
            byte_rx = UART_receive_byte();
            UART_transmit_byte(byte_rx);
        }
    }
}
