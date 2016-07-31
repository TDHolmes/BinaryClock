#include "LEDs.h"
#include "timer.h"
#include "ports.h"
#include "LEDs_drvr_STP16CPC26.h"
#include <stdint.h>

// Updates the LEDs
void LEDs_update_time(rtc_time_t *t_ptr, led_color_t *c_ptr, uint8_t display_column)
{
    uint8_t bits;
    uint32_t color_mask = 0;
    uint16_t row = 0;
    uint16_t red_set;
    uint16_t green_set;
    uint16_t blue_set;
    uint32_t row_mask;
    uint8_t col_mask;
    // get if we should display red, green, or blue this round
    red_set   = (c_ptr->red   & c_ptr->color_bit) << c_ptr->color_bit;
    green_set = (c_ptr->green & c_ptr->color_bit) << c_ptr->color_bit;
    blue_set  = (c_ptr->blue  & c_ptr->color_bit) << c_ptr->color_bit;
    // generate a color mask to apply to the active rows
    color_mask  =  red_set       | (green_set << 1)  | (blue_set << 2);
    color_mask |= (red_set << 3) | (green_set << 4)  | (blue_set << 5);
    color_mask |= (red_set << 6) | (green_set << 7)  | (blue_set << 8);
    color_mask |= (red_set << 9) | (green_set << 10) | (blue_set << 11);
    // get the time row and column masks
    LEDs_drvr_get_masks(t_ptr, display_column, &row_mask, &col_mask);

    // update the LEDs via whatever driver chip/method we're using
    LEDs_update_drvr((row_mask & color_mask), col_mask);
}

LEDs_update_manual(uint32_t row_mask, uint8_t display_column)
{

}
