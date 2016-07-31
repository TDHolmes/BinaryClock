/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include <avr/io.h>
#include "timer.h"
#include "RTC.h"
#include "LEDs.h"
#include "UART.h"
#include "hardware.h"
#include "timer.h"
#include "color.h"
#include "ports.h"
#include <stdint.h>

// private function declarations
void set_uart_flag(void);
void set_timer_expired_flag(void);
void clear_timer_expired_flag(void);
uint8_t increment_time(rtc_time_t *rtc_time_ptr);
uint8_t init_hardware(uint8_t *RTC_interrupt_ptr, uint8_t *timer_expired_ptr, led_color_t *color_struct_ptr,
                      rtc_time_t *time_ptr);

// states
typedef enum {
    run_time,
    run_manual
} state_t;

state_t state = WAIT;
uint8_t uart_command_received = 0; // valid command received flag
uint8_t timer_expired = 0;         // 15-20ish ms timer timeout flag
uint8_t command_buffer[COMMAND_LENGTH];  // where UART commands are buffered


int main(void)
{
    led_color_t colors;
    led_color_t *colors_ptr;
    rtc_time_t time;
    rtc_time_t *time_ptr;
    uint8_t retval = 0;
    uint8_t color_bit = 7;             // index which color bit we're displaying
    uint8_t RTC_interrupt_count = 0;   // 1 Hz RTC interrupt flag
    colors_ptr = &colors;
    time_ptr = &time;
    // initialize everything
    retval = init_hardware(&RTC_interrupt_count, &timer_expired, colors_ptr, time_ptr);

    ///// main while loop /////
    while(1) {
        // check if we have UART data to handle
        if (UART_receive_has_data() >= COMMAND_LENGTH) {
            state = run_handle_UART_comms;
        }

        switch(state) {
            case run_time: {
                if (RTC_interrupt_count != 0) {
                    increment_time(time_ptr, RTC_interrupt_count);
                    RTC_interrupt_count = 0;
                }
                if (LEDs_multiplex_timer_count != 0) {
                    LEDs_run()
                }
                break;
            }

            case run_manual: {

                break;
            }

            case run_handle_UART_comms: {
                UART_receive(command_buffer, COMMAND_LENGTH);
                // procress the received command
                break;
            }

            default: {

                break;
            }
        }
    }
}

void set_uart_flag(void)
{
    uart_command_received = 1;
}

void set_timer_expired_flag(void) 
{
    timer_expired = 1;
}

void clear_timer_expired_flag(void)
{
    timer_expired = 0;
}

uint8_t increment_time(rtc_time_t *rtc_time_ptr, inc_ammount)
{
    rtc_time_ptr->sec += inc_ammount;
    if (rtc_time_ptr->sec >= 60) {
        rtc_time_ptr->sec -= 60;
        rtc_time_ptr->min += 1;
        if (rtc_time_ptr->min >= 60) {
            rtc_time_ptr->min -= 60;
            rtc_time_ptr->hour += 1;
            if (rtc_time_ptr->hour >= 24) {
                rtc_time_ptr->hour -= 24;
            }
        }
    }
    return 0;
}

uint8_t init_hardware(uint8_t *RTC_interrupt_ptr, uint8_t *timer_expired_ptr, led_color_t *color_struct_ptr,
                      rtc_time_t *time_ptr)
{
    return 0;
}
