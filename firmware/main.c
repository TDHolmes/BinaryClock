/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include <avr/io.h>
#include "timer.h"
#include "RTC_DS3231.h"
#include "LED.h"
#include "UART.h"
#include "hardware.h"
#include "timer.h"
#include "color.h"
#include "ports.h"
#include <stdint.h>

#define COMMAND_LENGTH 6

// private function declarations
void     set_uart_flag(void);
void     set_timer_expired_flag(void);
void     clear_timer_expired_flag(void);
void     process_UART_command(rtc_time_t *time_ptr, uint8_t *cmd_buffer);
uint8_t  increment_time(rtc_time_t *rtc_time_ptr, uint8_t inc_ammount);

// states
typedef enum {
    RUN_TIME,
    RUN_MANUAL,
    RUN_HANDLE_UART
} state_t;

state_t state = RUN_TIME;
uint8_t uart_command_received = 0; // valid command received flag
uint8_t timer_expired = 0;         // 15-20ish ms timer timeout flag
uint8_t LED_multiplex_timer_count = 0;   // count of mutiplex handler timer events
uint8_t command_buffer[COMMAND_LENGTH];  // where UART commands are buffered


// Summary - 
// retval (int) - 
int main(void)
{
    led_color_t colors;
    led_color_t *colors_ptr;
    rtc_time_t time;
    rtc_time_t *time_ptr;
    uint8_t retval = 0;
    uint8_t RTC_interrupt_count = 0;   // 1 Hz RTC interrupt flag
    colors_ptr = &colors;
    time_ptr = &time;
    // initialize everything
    retval = init_hardware();

    ///// main while loop /////
    while(1) {
        // check if we have UART data to handle
        if (UART_receive_has_data() >= COMMAND_LENGTH) {
            state = RUN_HANDLE_UART;
        }

        switch(state) {
            case RUN_TIME: {
                if (RTC_interrupt_count != 0) {
                    increment_time(time_ptr, RTC_interrupt_count);
                    RTC_interrupt_count = 0;
                }
                if (LED_multiplex_timer_count != 0) {
                    LED_run(LED_multiplex_timer_count);
                }
                break;
            }

            case RUN_MANUAL: {

                break;
            }

            case RUN_HANDLE_UART: {
                UART_receive(command_buffer, COMMAND_LENGTH);
                // procress the received command
                process_UART_command(time_ptr, command_buffer);
                break;
            }

            default: {

                break;
            }
        }
    }
}


// Summary - 
// param (rtc_time_t *) time_ptr - 
// param (uint8_t *) cmd_buffer - 
void process_UART_command(rtc_time_t *time_ptr, uint8_t *cmd_buffer)
{
    switch(cmd_buffer[0]) {
        case UART_CMD_SET_LED: {
            if (state != RUN_MANUAL) {
                UART_transmit_byte(UART_CMD_WRONG_STATE);
                break;
            }
            // bytes 1 & 2 is row & column, bytes 3, 4, & 5 is red, green, and blue vals
            LED_set(cmd_buffer[1], cmd_buffer[2], cmd_buffer[3], cmd_buffer[4], cmd_buffer[5]);
            UART_transmit_byte(UART_CMD_VALID);
            break;
        }

        case UART_CMD_CLEAR_LED: {
            if (state != RUN_MANUAL) {
                UART_transmit_byte(UART_CMD_WRONG_STATE);
                break;
            }
            // bytes 1 & 2 is row & column
            LED_clear(cmd_buffer[1], cmd_buffer[2]);
            UART_transmit_byte(UART_CMD_VALID);
            break;
        }

        case UART_CMD_CLEAR_ALL_LED: {
            if (state != RUN_MANUAL) {
                UART_transmit_byte(UART_CMD_WRONG_STATE);
                break;
            }
            // clear all LEDs
            LED_clear_all();
            UART_transmit_byte(UART_CMD_VALID);
            break;
        }

        case UART_CMD_SET_ALL_LED: {
            if (state != RUN_MANUAL) {
                UART_transmit_byte(UART_CMD_WRONG_STATE);
                break;
            }
            // Bytes 1, 2, and 3 is red, green and blue values
            LED_set_all(cmd_buffer[1], cmd_buffer[2], cmd_buffer[3]);
            UART_transmit_byte(UART_CMD_VALID);
            break;
        }

        case UART_CMD_SET_TIME: {
            if (state != RUN_TIME) {
                UART_transmit_byte(UART_CMD_WRONG_STATE);
                break;
            }
            // Byte 1: Hour, Byte 2: Min, Byte 3: sec
            RTC_set_time(time_ptr, cmd_buffer[1], cmd_buffer[2], cmd_buffer[3]);
            UART_transmit_byte(UART_CMD_VALID);
            break;
        }

        case UART_CMD_SET_COLOR: {
            if (state != RUN_TIME) {
                UART_transmit_byte(UART_CMD_WRONG_STATE);
                break;
            }
            // Byte 1: Red, Byte 2: Green, Byte 3: Blue
            LED_set_color(cmd_buffer[1], cmd_buffer[2], cmd_buffer[3]);
            UART_transmit_byte(UART_CMD_VALID);
            break;
        }

        case UART_CMD_CHANGE_STATE: {
            if (cmd_buffer[1] == 0) {
                state = RUN_TIME;
                LED_update_time(time_ptr);
            } else if (cmd_buffer[1] == 1) {
                state = RUN_MANUAL;
                LED_clear_all();
            }
            UART_transmit_byte(UART_CMD_VALID);
            break;
        }

        default: {
            UART_transmit_byte(UART_CMD_INVALID);
            break;
        }
    }
}

// Summary - 
void set_uart_flag(void)
{
    uart_command_received = 1;
}

// Summary - 
void set_timer_expired_flag(void) 
{
    timer_expired = 1;
}

// Summary - 
void clear_timer_expired_flag(void)
{
    timer_expired = 0;
}

// Summary - 
// param (rtc_time_t *) rtc_time_ptr - 
// param (uint8_t) inc_ammount - 
// retval (uint8_t) - 
uint8_t increment_time(rtc_time_t *rtc_time_ptr, uint8_t inc_ammount)
{
    rtc_time_ptr->second += inc_ammount;
    if (rtc_time_ptr->second >= 60) {
        rtc_time_ptr->second -= 60;
        rtc_time_ptr->minute += 1;
        if (rtc_time_ptr->minute >= 60) {
            rtc_time_ptr->minute -= 60;
            rtc_time_ptr->hour += 1;
            if (rtc_time_ptr->hour >= 24) {
                rtc_time_ptr->hour -= 24;
            }
        }
    }
    return 0;
}
