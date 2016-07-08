
#define CMD_UPDATE_TIME  0x42
#define CMD_UPDATE_COLOR 0x43
#define CMD_INVALID      0x00

enum {
    WAITING_FOR_COMMAND,
    WAITING_FOR_BYTE_0,
    WAITING_FOR_BYTE_1,
    WAITING_FOR_BYTE_2
}

struct uart_command {
    int command;
    int byte_0;
    int byte_1;
    int byte_2;
} uart_command;

static int uart_state;

void init_uart(void)
{
    uart_state = WAITING_FOR_COMMAND;
    uart_command = {CMD_INVALID, 0, 0, 0};
}

void uart_interrupt(void)
{
    switch(uart_state) {
        case WAITING_FOR_COMMAND: {
            uart_command.command = data;
            uart_state = WAITING_FOR_BYTE_0
            break;
        }

        case WAITING_FOR_BYTE_0: {
            uart_command.byte_0 = data;
            uart_state = WAITING_FOR_BYTE_1
            break;
        }

        case WAITING_FOR_BYTE_1: {
            uart_command.byte_1 = data;
            uart_state = WAITING_FOR_BYTE_2
            break;
        }

        case WAITING_FOR_BYTE_2: {
            uart_command.byte_2 = data;
            uart_state = WAITING_FOR_COMMAND;
            // Tell main that we have a valid UART message with data
            break;
        }

    }
}

void get_uart_command_data(int *command_ptr, int *byte0_ptr, int *byte1_ptr, int *byte2_ptr)
{
    *command_ptr = uart_command.command;
    *byte0_ptr = uart_command.byte_0;
    *byte1_ptr = uart_command.byte_1;
    *byte2_ptr = uart_command.byte_2;
}
