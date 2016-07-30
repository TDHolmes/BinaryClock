#include "LEDs.h"
#include "timer.h"
#include "ports.h"
#include <stdint.h>

void update_LEDs(rtc_time_t *t_ptr, led_color_t *c_ptr)
{
    uint8_t bits;
    uint32_t color_mask = 0;
    uint16_t row = 0;
    uint16_t red_set;
    uint16_t green_set;
    uint16_t blue_set;
    // get if we should display red, green, or blue this round
    red_set   = (c_ptr->red   & c_ptr->color_bit) << c_ptr->color_bit;
    green_set = (c_ptr->green & c_ptr->color_bit) << c_ptr->color_bit;
    blue_set  = (c_ptr->blue  & c_ptr->color_bit) << c_ptr->color_bit;
    // generate a color mask to and to the active rows
    color_mask  =  red_set       | (green_set << 1)  | (blue_set << 2);
    color_mask |= (red_set << 3) | (green_set << 4)  | (blue_set << 5);
    color_mask |= (red_set << 6) | (green_set << 7)  | (blue_set << 8);
    color_mask |= (red_set << 9) | (green_set << 10) | (blue_set << 11);
    // apply the color mask to the active rows
    row = color_mask;
    // clock out data to LED driver
    // unroll loop if that's a pragma option
    for(bits = 20; bits != 0; bits -= 1) {
        LEDDRV_CLK(0);
        LEDDRV_SDI(row & 0x01);
        row = row >> 1;
        LEDDRV_CLK(1);
    }
    // Latch the LED driver data
    LEDDRV_LE = 1;
    LEDDRV_LE = 0;
    // Turn off LEDs until high side is set (active low output enable)
    LEDDRV_OE = 1;
    // Enable the anode high side
    ANO_PORT = ~((column & 0b00011111) | ((column & 0b00100000) << 1));
    // output the LED driver data
    LEDDRV_OE = 0;
}
