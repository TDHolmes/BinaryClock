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


#define NO_FORCE_UPDATE 0  //!< Only update the LED array columns that have changed.
#define FORCE_UPDATE    1  //!< Update all LED array columns.

#define COMMAND_LENGTH 6   //!< number of bytes in a valid UART command.

//! Current state of the clock.
typedef enum {
    RUN_TIME,   //!< Runs time display mode. LEDs controlled by the current time.
    RUN_MANUAL  //!< Runs manual display mode. LEDs controlled by UART commands.
} state_t;

state_t system_state = RUN_TIME;   //!< keeps track of the system state.
uint8_t uart_command_received = 0; //!< valid command received flag.
uint8_t timer_expired = 0;         //!< 15-20ish ms timer timeout flag.
uint8_t LED_multiplex_timer_count = 0;   //!< count of mutiplex handler timer events.
uint8_t command_buffer[COMMAND_LENGTH];  //!< where UART commands are buffered.

// private function declarations
static void process_UART_command(rtc_time_t *time_ptr, uint8_t *cmd_buffer);
static void increment_time(rtc_time_t *rtc_time_ptr, uint8_t inc_ammount);


/*!
 * Runs the main program.
 *
 * Order of opperation:
 *   1. Check if we've received enough data for a command.
 *     - If we have, process it.
 *   2. Check if we should run manual control or time control.
 *     - If manual control:
 *        a. run the LED multiplex function.
 *        b. keep time as well.
 *     - If time control:
 *        a. check if we need to update the time
 *           - if so, increment time and update the LED array
 *        b. run the LED multiplex function
 */
int main(void)
{
    volatile uint8_t multiplexer_count;
    volatile uint8_t rtc_1hz_int_count;
    uint8_t time_diff;
    uint8_t time_updated = FALSE;
    uint8_t start_byte_received = FALSE;
    retval_t retval = GEN_FAIL;
    rtc_time_t time;
    rtc_time_t *t_ptr = &time;
    t_ptr->second = 0;
    t_ptr->minute = 1;
    t_ptr->hour = 2;
    hardware_init();
    LED_init();
    LED_set_color(1, 1, 1);
    timer_init(&multiplexer_count);
    UART_init((uint32_t)250000);
    i2c_init();
    RTC_init(t_ptr, &rtc_1hz_int_count);

    ///// main while loop /////
    while(1) {
        // check if we have UART data to handle
        if (UART_receive_unread_items() >= COMMAND_LENGTH) {
            // check if we haven't received a start byte yet
            if (start_byte_received == FALSE) {
                // check for the start byte
                do {
                    UART_receive_byte(command_buffer, TRUE);
                    if (command_buffer[0] == UART_CMD_START_BYTE) {
                        start_byte_received = TRUE;
                        break;
                    }
                } while(UART_receive_unread_items() >= 1);
            } else {
                // We've received the start byte. Handle the command (if enough data)
                if (UART_receive_unread_items() >= COMMAND_LENGTH) {
                    // get the data
                    UART_receive(command_buffer, COMMAND_LENGTH);
                    // procress the received command
                    process_UART_command(t_ptr, command_buffer);
                    start_byte_received = FALSE;  // reset start byte
                }
            }
        }

        // run the system
        switch(system_state) {
            case RUN_TIME: {
                if (rtc_1hz_int_count != 0) {
                    time_diff = rtc_1hz_int_count;
                    rtc_1hz_int_count = 0;
                    increment_time(t_ptr, time_diff);
                    LED_update_time(t_ptr, TRUE);
                }
                if (multiplexer_count != 0) {
                    multiplexer_count = 0;
                    LED_run();
                }
                // every day, re-check with RTC to update time. Will try for a minute if unsuccessful
                if (t_ptr->hour == 3 && t_ptr->minute == 0 && time_updated == FALSE) {
                    retval = RTC_read_time(t_ptr);
                    if (retval == GEN_PASS) {
                        time_updated = TRUE;
                    }
                } else if (t_ptr->hour == 3 && t_ptr->minute == 1) {
                    // reset it for tomorrow
                    time_updated = FALSE;
                }
                break;
            }

            case RUN_MANUAL: {
                // let UART commands update the LEDs, but continue to refresh them
                if (multiplexer_count >= 1) {
                    multiplexer_count = 0;
                    LED_run();
                }
                // but keep track of the time still
                if (rtc_1hz_int_count >= 1) {
                    time_diff = rtc_1hz_int_count;
                    rtc_1hz_int_count = 0;
                    increment_time(t_ptr, time_diff);
                }
                break;
            }

            default: {

                break;
            }
        }
    }
}


/*!
 * Processes a received UART command. Here's a list of commands:
 *   1. UART_CMD_SET_LED:
 *       - Sets an individual LED to a certain color. Must be in manual
 *         control mode.
 *   1. UART_CMD_CLEAR_LED:
 *       - Clears an individual LED. Must be in manual control mode.
 *   1. UART_CMD_CLEAR_ALL_LED:
 *       - Clears all LEDs. Must be in manual control mode.
 *   1. UART_CMD_SET_ALL_LED:
 *       - Sets all LEDs to a certain color. Must be in manual
 *         control mode.
 *   1. UART_CMD_SET_TIME:
 *       - Sets the time. Must be in time control mode.
 *   1. UART_CMD_SET_COLOR:
 *       - Sets the color of the LEDs when running in time control mode.
 *   1. UART_CMD_CHANGE_STATE:
 *       - Changes the state the board is running in.
 * 
 * @param[in] time_ptr (rtc_time_t *): Pointer that is used to keep track of the time.
 * @param[in] cmd_buffer (uint8_t *): Buffer that holds the bytes in the command.
 */
void process_UART_command(rtc_time_t *time_ptr, uint8_t *cmd_buffer)
{
    switch(cmd_buffer[0]) {
        case UART_CMD_SET_LED: {
            if (system_state != RUN_MANUAL) {
                UART_transmit_byte(UART_CMD_WRONG_STATE, TRUE);
                break;
            }
            // bytes 1 & 2 is row & column, bytes 3, 4, & 5 is red, green, and blue vals
            LED_set(cmd_buffer[1], cmd_buffer[2], cmd_buffer[3], cmd_buffer[4], cmd_buffer[5]);
            UART_transmit_byte(UART_CMD_VALID, TRUE);
            break;
        }

        case UART_CMD_CLEAR_LED: {
            if (system_state != RUN_MANUAL) {
                UART_transmit_byte(UART_CMD_WRONG_STATE, TRUE);
                break;
            }
            // bytes 1 & 2 is row & column
            LED_clear(cmd_buffer[1], cmd_buffer[2]);
            UART_transmit_byte(UART_CMD_VALID, TRUE);
            break;
        }

        case UART_CMD_CLEAR_ALL_LED: {
            if (system_state != RUN_MANUAL) {
                UART_transmit_byte(UART_CMD_WRONG_STATE, TRUE);
                break;
            }
            // clear all LEDs
            LED_clear_all();
            UART_transmit_byte(UART_CMD_VALID, TRUE);
            break;
        }

        case UART_CMD_SET_ALL_LED: {
            if (system_state != RUN_MANUAL) {
                UART_transmit_byte(UART_CMD_WRONG_STATE, TRUE);
                break;
            }
            // Bytes 1, 2, and 3 is red, green and blue values
            LED_set_all(cmd_buffer[1], cmd_buffer[2], cmd_buffer[3]);
            UART_transmit_byte(UART_CMD_VALID, TRUE);
            break;
        }

        case UART_CMD_SET_TIME: {
            if (system_state != RUN_TIME) {
                UART_transmit_byte(UART_CMD_WRONG_STATE, TRUE);
                break;
            }
            // Byte 1: Hour, Byte 2: Min, Byte 3: sec
            RTC_set_time(time_ptr, cmd_buffer[1], cmd_buffer[2], cmd_buffer[3]);
            UART_transmit_byte(UART_CMD_VALID, TRUE);
            break;
        }

        case UART_CMD_SET_COLOR: {
            if (system_state != RUN_TIME) {
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
                system_state = RUN_TIME;
                LED_update_time(time_ptr, FORCE_UPDATE);
            } else if (cmd_buffer[1] == 1) {
                system_state = RUN_MANUAL;
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


/*!
 * Increments the time admin pointer to keep track of the time.
 * 
 * @param[in] time_ptr (rtc_time_t *): Pointer that is used to keep track of the time.
 * @param[in] inc_ammount (uint8_t): Ammount to increment by.
 */
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
