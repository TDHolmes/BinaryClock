#ifndef LED_H_
#define LED_H_

#include "color.h"
#include "hardware.h"
#include "RTC_DS3231.h"
#include <stdint.h>
#include <avr/io.h>

/*! \file LED.h
    \brief High level LED definitions.
*/

#define DISP_UPDATE_FREQ 60   //!< update frequency
#define COLS 6                //!< number of columns
#define ROWS (COLOR_BITS * 4) //!< number of rows
//! number of clock cycles in between multiplexer cycles
#define MULTIPLEX_TIMER_CLK_CYCLES (F_OSC / (DISP_UPDATE_FREQ * COLS * COLOR_BITS))


void LED_init(void);
void LED_update_time(rtc_time_t *t_ptr, uint8_t update_all_override);
void LED_run(void);
void LED_clear(uint8_t row, uint8_t col);
void LED_set(uint8_t row, uint8_t col, uint8_t red, uint8_t green, uint8_t blue);
void LED_clear_all(void);
void LED_set_all(uint8_t red, uint8_t blue, uint8_t green);
void LED_set_color(uint8_t red, uint8_t blue, uint8_t green);

#endif /* LED_H_ */
