#include "LED.h"
#include "timer.h"
#include "ports.h"
#include "LED_drvr_STP16CPC26.h"
#include <stdint.h>

LED_drvr_t LED_admin_struct;
LED_drvr_t *LED_admin_struct_ptr;

LED_init()
{
    LED_admin_struct_ptr = &LED_admin_struct;
    LED_drvr_init(LED_admin_struct_ptr);
}

// update LED based on a time structure
LED_update_time(time_struct)
{

}

// called cyclicly in while loop
LED_run(LED_multiplex_timer_count)
{
    LED_drvr_run(LED_drvr_t *LED_admin_struct_ptr, LED_multiplex_timer_count);
}

// returns the accrued errors
LED_errors_check(LED_error_struct_ptr)
{

}

// resets any errors
LED_errors_reset()
{

}

// clears the LED at the given index
LED_clear(uint8_t row, uint8_t col)
{

}

// sets the LED at the given index to the given color struct
LED_set(uint8_t row, uint8_t col, uint8_t red, uint8_t green, uint8_t blue)
{

}

// clears all LED
LED_clear_all()
{

}

// sets all LED to the given color
LED_set_all(uint8_t red, uint8_t blue, uint8_t green)
{

}


// // Updates the LED
// void LED_update_time(rtc_time_t *t_ptr, led_color_t *c_ptr, uint8_t display_column)
// {
//     uint8_t bits;
//     uint32_t color_mask = 0;
//     uint16_t row = 0;
//     uint16_t red_set;
//     uint16_t green_set;
//     uint16_t blue_set;
//     uint32_t row_mask;
//     uint8_t col_mask;
//     // get if we should display red, green, or blue this round
//     red_set   = (c_ptr->red   & c_ptr->color_bit) << c_ptr->color_bit;
//     green_set = (c_ptr->green & c_ptr->color_bit) << c_ptr->color_bit;
//     blue_set  = (c_ptr->blue  & c_ptr->color_bit) << c_ptr->color_bit;
//     // generate a color mask to apply to the active rows
//     color_mask  =  red_set       | (green_set << 1)  | (blue_set << 2);
//     color_mask |= (red_set << 3) | (green_set << 4)  | (blue_set << 5);
//     color_mask |= (red_set << 6) | (green_set << 7)  | (blue_set << 8);
//     color_mask |= (red_set << 9) | (green_set << 10) | (blue_set << 11);
//     // get the time row and column masks
//     LED_drvr_get_masks(t_ptr, display_column, &row_mask, &col_mask);

//     // update the LED via whatever driver chip/method we're using
//     LED_update_drvr((row_mask & color_mask), col_mask);
// }



// LED_update_manual(uint32_t row_mask, uint8_t display_column)
// {

// }