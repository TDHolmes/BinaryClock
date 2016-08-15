/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include "LED.h"
#include "hardware.h"
#include "color.h"
#include "ports.h"
#include "RTC_DS3231.h"
#include "timer_drvr_4313.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

rtc_time_t time_struct;
rtc_time_t *t_ptr;

#define NO_FORCE_UPDATE 0
#define FORCE_UPDATE    1


uint8_t increment_time(rtc_time_t *rtc_time_ptr, uint8_t inc_ammount);

// Summary - 
// retval (int) - 
int main(void)
{
    uint16_t i = 0;
    uint8_t enter_update = 1;
    uint8_t red, green, blue;
    uint8_t multiplier_count = 0;
    uint32_t millis_count = 0;
    red = 0;
    green = 0;
    blue = 0;
    hardware_init();
    LED_init();

    timer_init(&multiplier_count);
    time_struct.second = 20;
    time_struct.minute = 42;
    time_struct.hour = 16;
    t_ptr = &time_struct;
    LED_set_color(red, green, blue);
    LED_update_time(t_ptr, FORCE_UPDATE);
    // turn on all interrupts
    sei();
    ///// main while loop /////
    while(1) {
        millis_count = timer_millis_drvr_get();
        // check if we've gone 1 second since last update & we haven't
        // already incremented the time
        if (millis_count % 1000 == 1 && enter_update == 1) {
            enter_update = 0;
            increment_time(t_ptr, 1);
            LED_update_time(t_ptr, NO_FORCE_UPDATE);
        } else if (millis_count % 1000 == 2) {
            // re-enable the updating of time
            enter_update = 1;
        }
        if (millis_count % 200 == 0) {
            red += 1;
            if (red == 16) {
                red = 0;
                blue += 1;
                if (blue == 16) {
                    green += 1;
                    if (green == 16) {
                        green = 0;
                    }
                }
            }
            LED_set_color(red, green, blue);
            LED_update_time(t_ptr, FORCE_UPDATE);
        }
        // run the LED runner to multiplex the LEDs
        if (multiplier_count != 0) {
            LED_run(multiplier_count);
            multiplier_count = 0;
        }
    }
}


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

