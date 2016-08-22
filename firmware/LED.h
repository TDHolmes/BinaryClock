#ifndef LED_H_
#define LED_H_

#include "color.h"
#include "hardware.h"
#include "RTC_DS3231.h"
#include <stdint.h>
#include <avr/io.h>

#define ROW_8_R    0b000000000001
#define ROW_8_G    0b000000000010
#define ROW_8_B    0b000000000100
#define ROW_4_R    0b000000001000
#define ROW_4_G    0b000000010000
#define ROW_4_B    0b000000100000
#define ROW_2_R    0b000001000000
#define ROW_2_G    0b000010000000
#define ROW_2_B    0b000100000000
#define ROW_1_R    0b001000000000
#define ROW_1_G    0b010000000000
#define ROW_1_B    0b100000000000

#define COL_H_TENS 0b000001
#define COL_H_ONES 0b000010
#define COL_M_TENS 0b000100
#define COL_M_ONES 0b001000
#define COL_S_TENS 0b010000
#define COL_S_ONES 0b100000

#define DISP_HOUR_TENS 0
#define DISP_HOUR_ONES 1
#define DISP_MIN_TENS  2
#define DISP_MIN_ONES  3
#define DISP_SEC_TENS  4
#define DISP_SEC_ONES  5

#define DISP_UPDATE_FREQ 60
#define COLS 6
#define ROWS (COLOR_BITS * 4)
#define MULTIPLEX_TIMER_CLK_CYCLES (F_OSC / (DISP_UPDATE_FREQ * COLS * COLOR_BITS))

typedef struct {
    uint8_t missed_multiplex_timer_interrupts;
} LED_errors_t;

void LED_init(void);
void LED_update_time(rtc_time_t *t_ptr, uint8_t update_all_override);
void LED_run(uint8_t LED_multiplex_timer_count);
void LED_clear(uint8_t row, uint8_t col);
void LED_set(uint8_t row, uint8_t col, uint8_t red, uint8_t green, uint8_t blue);
void LED_clear_all(void);
void LED_set_all(uint8_t red, uint8_t blue, uint8_t green);
void LED_set_color(uint8_t red, uint8_t blue, uint8_t green);

#endif /* LED_H_ */
