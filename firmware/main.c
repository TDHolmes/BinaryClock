/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include <avr/io.h>
#include "timer_4313.h"
#include "RTC_DS3231.h"
#include "LED.h"
#include "UART.h"
#include "i2c.h"
#include "hardware.h"
#include "color.h"
#include "ports.h"
#include "global_defines.h"
#include <stdint.h>


#define NO_FORCE_UPDATE 0
#define FORCE_UPDATE    1

#define COMMAND_LENGTH 6

// private function declarations
static void process_UART_command(rtc_time_t *time_ptr, uint8_t *cmd_buffer);
static void increment_time(rtc_time_t *rtc_time_ptr, uint8_t inc_ammount);
static void wait_time(uint32_t time);

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

uint8_t vals_rx[16];

// Summary - 
// retval (int) - 
int main(void)
{
    retval_t retval;
    volatile uint8_t multiplexer_count;
    volatile uint8_t rtc_1hz_int_count;
    uint8_t red, green, blue = 0;
    rtc_time_t time;
    rtc_time_t *t_ptr = &time;
    t_ptr->second = 0;
    t_ptr->minute = 1;
    t_ptr->hour = 2;
    hardware_init();
    LED_init();
    LED_set_color(3, 3, 3);
    timer_init(&multiplexer_count);
    UART_init((uint32_t)9600);
    retval = i2c_init();
    retval = RTC_init(t_ptr, &rtc_1hz_int_count);
    RTC_set_time(t_ptr, 21, 40, 45);
    while (1) {
        if (rtc_1hz_int_count >= 1) {
            increment_time(t_ptr, rtc_1hz_int_count);
            rtc_1hz_int_count = 0;
            LED_update_time(t_ptr, TRUE);
        }
        if (multiplexer_count >= 1) {
            LED_run(multiplexer_count);
            multiplexer_count = 0;
            LED_update_time(t_ptr, TRUE);
        }
    }

    ///// main while loop /////
    while(1) {
        // check if we have UART data to handle
        // if (UART_receive_has_data() >= COMMAND_LENGTH) {
        //     // procress the received command
        //     process_UART_command(time_ptr, command_buffer);
        // }

        // switch(state) {
        //     case RUN_TIME: {
        //         // if (timer_millis_get() % 1000 == 0 && enter_update_time == 1) {
        //         //     enter_update_time = 0;
        //         //     increment_time(time_ptr, 1);
        //         //     LED_update_time(time_ptr, 0);
        //         // } else if (timer_millis_get() % 1000 == 1) {
        //         //     enter_update_time = 1;
        //         // }
        //         if (RTC_interrupt_count != 0) {
        //             tempVal = RTC_interrupt_count;
        //             increment_time(time_ptr, RTC_interrupt_count);
        //             RTC_interrupt_count -= tempVal;
        //         }
        //         if (LED_multiplex_timer_count != 0) {
        //             tempVal = LED_multiplex_timer_count;
        //             LED_run(tempVal);
        //             LED_multiplex_timer_count -= tempVal;
        //         }
        //         break;
        //     }

        //     case RUN_MANUAL: {
        //         // let UART commands set LEDs
        //         break;
        //     }

        //     default: {

        //         break;
        //     }
        // }
    }
}


void wait_time(uint32_t time)
{
    uint32_t i;
    for (i = 0; i < time; i++);
}


// Summary - 
// param (rtc_time_t *) time_ptr - 
// param (uint8_t *) cmd_buffer - 
void process_UART_command(rtc_time_t *time_ptr, uint8_t *cmd_buffer)
{
    switch(cmd_buffer[0]) {
        case UART_CMD_SET_LED: {
            if (state != RUN_MANUAL) {
                UART_transmit_byte(UART_CMD_WRONG_STATE, TRUE);
                break;
            }
            // bytes 1 & 2 is row & column, bytes 3, 4, & 5 is red, green, and blue vals
            LED_set(cmd_buffer[1], cmd_buffer[2], cmd_buffer[3], cmd_buffer[4], cmd_buffer[5]);
            UART_transmit_byte(UART_CMD_VALID, TRUE);
            break;
        }

        case UART_CMD_CLEAR_LED: {
            if (state != RUN_MANUAL) {
                UART_transmit_byte(UART_CMD_WRONG_STATE, TRUE);
                break;
            }
            // bytes 1 & 2 is row & column
            LED_clear(cmd_buffer[1], cmd_buffer[2]);
            UART_transmit_byte(UART_CMD_VALID, TRUE);
            break;
        }

        case UART_CMD_CLEAR_ALL_LED: {
            if (state != RUN_MANUAL) {
                UART_transmit_byte(UART_CMD_WRONG_STATE, TRUE);
                break;
            }
            // clear all LEDs
            LED_clear_all();
            UART_transmit_byte(UART_CMD_VALID, TRUE);
            break;
        }

        case UART_CMD_SET_ALL_LED: {
            if (state != RUN_MANUAL) {
                UART_transmit_byte(UART_CMD_WRONG_STATE, TRUE);
                break;
            }
            // Bytes 1, 2, and 3 is red, green and blue values
            LED_set_all(cmd_buffer[1], cmd_buffer[2], cmd_buffer[3]);
            UART_transmit_byte(UART_CMD_VALID, TRUE);
            break;
        }

        case UART_CMD_SET_TIME: {
            if (state != RUN_TIME) {
                UART_transmit_byte(UART_CMD_WRONG_STATE, TRUE);
                break;
            }
            // Byte 1: Hour, Byte 2: Min, Byte 3: sec
            RTC_set_time(time_ptr, cmd_buffer[1], cmd_buffer[2], cmd_buffer[3]);
            UART_transmit_byte(UART_CMD_VALID, TRUE);
            break;
        }

        case UART_CMD_SET_COLOR: {
            if (state != RUN_TIME) {
                UART_transmit_byte(UART_CMD_WRONG_STATE, TRUE);
                break;
            }
            // Byte 1: Red, Byte 2: Green, Byte 3: Blue
            LED_set_color(cmd_buffer[1], cmd_buffer[2], cmd_buffer[3]);
            UART_transmit_byte(UART_CMD_VALID, TRUE);
            break;
        }

        case UART_CMD_CHANGE_STATE: {
            if (cmd_buffer[1] == 0) {
                state = RUN_TIME;
                LED_update_time(time_ptr, FORCE_UPDATE);
            } else if (cmd_buffer[1] == 1) {
                state = RUN_MANUAL;
                LED_clear_all();
            }
            UART_transmit_byte(UART_CMD_VALID, TRUE);
            break;
        }

        default: {
            UART_transmit_byte(UART_CMD_INVALID, TRUE);
            break;
        }
    }
}


// Summary - 
// param (rtc_time_t *) rtc_time_ptr - 
// param (uint8_t) inc_ammount - 
// retval (uint8_t) - 
void increment_time(rtc_time_t *rtc_time_ptr, uint8_t inc_ammount)
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
}
