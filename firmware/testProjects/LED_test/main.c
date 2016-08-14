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
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

rtc_time_t time_struct;
rtc_time_t *t_ptr;


uint8_t increment_time(rtc_time_t *rtc_time_ptr, uint8_t inc_ammount);

// Summary - 
// retval (int) - 
int main(void)
{
    uint16_t i;
    uint8_t red, green, blue;
    red = 0;
    green = 0;
    blue = 0;
    hardware_init();
    LED_init();
    time_struct.second = 42;
    time_struct.minute = 30;
    time_struct.hour = 14;
    t_ptr = &time_struct;
    LED_set_color(15, 4, 8);
    LED_update_time(t_ptr, 1);
    ///// main while loop /////
    while(1) {
        LED_update_time(t_ptr, 0);
        for (i = 0; i < 700 * 8; i++) {
            LED_run(1);
            if (i % 700 == 0) {
                red += 1;
                if (red == 16) {
                    red = 0;
                    green += 1;
                    if (green == 16) {
                        green = 0;
                        blue += 1;
                        if (blue == 16) {
                            blue = 0;
                        }
                    }
                }
                LED_set_color(red, green, blue);
                LED_update_time(t_ptr, 1);
            }
        }
        increment_time(t_ptr, 1);
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

