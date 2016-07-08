#include "LEDs.h"
#include "ports.h"

void update_LEDs(int row, int column, int red, int green, int blue, int color_bit)
{
    int bits;
    int color_mask = 0;
    // get if we should display red, green, or blue this round
    red = (red & color_bit) << color_bit;
    green = (green & color_bit) << color_bit;
    blue = (blue & color_bit) << color_bit;
    // generate a color mask to and to the active rows
    color_mask = red | (green >> 1) | (blue >> 2);
    color_mask = color_mask | (red >> 3) | (green >> 4) | (blue >> 5);
    color_mask = color_mask | (red >> 6) | (green >> 7) | (blue >> 8);
    color_mask = color_mask | (red >> 9) | (green >> 10) | (blue >> 11);
    // apply the color mask to the active rows
    row = row & color_mask;
    // clock out data to LED driver
    // unroll loop if that's a pragma option
    for(bits = 20; bits != 0; bits -= 1) {
        LEDDRV_CLK = 0;
        LEDDRV_SDI = row;
        row = row << 1;
        LEDDRV_CLK = 1;
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
