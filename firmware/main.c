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
    HANDLE_UART,
    WAIT,
    HOUR_TENS,
    HOUR_ONES,
    MIN_TENS,
    MIN_ONES,
    SEC_TENS,
    SEC_ONES
} state_t;

state_t state = WAIT;
uint8_t uart_command_received = 0; // valid command received flag
uint8_t timer_expired = 0;         // 15-20ish ms timer timeout flag


int main(void)
{
    led_color_t colors;
    led_color_t *colors_ptr;
    rtc_time_t time;
    rtc_time_t *time_ptr;
    uint8_t retval = 0;
    uint8_t color_bit = 7;           // index which color bit we're displaying
    uint8_t RTC_interrupt = 0;       // 1 Hz RTC interrupt flag
    colors_ptr = &colors;
    time_ptr = &time;
    // initialize everything
    retval = init_hardware(&RTC_interrupt, &timer_expired, colors_ptr, time_ptr);
    if(retval != 0) {
        // error!
    } else {
        state = WAIT;
    }

    ///// main while loop /////
    while(1) {
        if(uart_command_received) {
            state = HANDLE_UART;  // jump to handling UART state
        }
        // Show to the user what state we're in (for debug)
        if(state == WAIT) {
            LED0_set(0);
            LED1_set(1);
        } else {
            LED0_set(1);
            LED1_set(0);
        }
        switch(state) {
            case WAIT: {
                // If we've got a 1 Hz RTC interrupt, time to start updating the LEDs
                if(RTC_interrupt) {
                    RTC_interrupt = 0;                  // reset interrupt flag
                    retval = increment_time(time_ptr);  // increment time!!
                    reset_timer();
                    color_bit = 7;
                    update_LEDs(time_ptr, colors_ptr);
                    LED0_set(1);
                    LED1_set(0);
                    state = HOUR_TENS;
                }
                break;
            }
            
            case HOUR_TENS: {
                if(timer_expired) {
                    reset_timer();
                    update_LEDs(time_ptr, colors_ptr);
                    state = HOUR_ONES;
                }
                break;
            }
            
            case HOUR_ONES: {
                if(timer_expired) {
                    reset_timer();
                    update_LEDs(time_ptr, colors_ptr);
                    state = MIN_TENS;
                }
                break;
            }
            
            case MIN_TENS: {
                if(timer_expired) {
                    reset_timer();
                    update_LEDs(time_ptr, colors_ptr);
                    state = MIN_ONES;
                }
                break;
            }
            
            case MIN_ONES: {
                if(timer_expired) {
                    reset_timer();
                    update_LEDs(time_ptr, colors_ptr);
                    state = SEC_TENS;
                }
                break;
            }
            
            case SEC_TENS: {
                if(timer_expired) {
                    reset_timer();
                    update_LEDs(time_ptr, colors_ptr);
                    state = SEC_ONES;
                }
                break;
            }
            
            case SEC_ONES: {
                if(color_bit == 0) {
                    state = WAIT;
                } else {
                    color_bit -= 1;
                    reset_timer();
                    update_LEDs(time_ptr, colors_ptr);
                    state = HOUR_TENS;
                }
                break;
            }
            
            case HANDLE_UART: {
                // handle UART command received
                state = WAIT;
                break;
            }
            
            default: {
                // error
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

uint8_t increment_time(rtc_time_t *rtc_time_ptr)
{
    rtc_time_ptr->sec += 1;
    if(rtc_time_ptr->sec >= 60) {
        rtc_time_ptr->sec -= 60;
        rtc_time_ptr->min += 1;
        if(rtc_time_ptr->min >= 60) {
            rtc_time_ptr->min -= 60;
            rtc_time_ptr->hour += 1;
            if(rtc_time_ptr->hour >= 24) {
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
