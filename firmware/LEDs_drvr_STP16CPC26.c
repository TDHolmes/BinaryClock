#include "LEDs_drvr_STP16CPC26.h"
#include "LEDs.h"
#include "ports.h"
#include <stdint.h>

#define ROW_SIZE 12
#define COL_SIZE 6

LEDs_drvr_update(uint16_t row_mask, uint8_t active_column)
{
    // apply the color & time masks to the active rows
    row = color_mask & row_mask;
    // clock out data to LED driver
    // unroll loop if that's a pragma option
    for(bits = 20; bits != 0; bits -= 1) {
        LEDDRV_CLK_SET(0);
        LEDDRV_SDI_SET(row & 0x01);
        row = row >> 1;
        LEDDRV_CLK_SET(1);
    }
    // Latch the LED driver data
    LEDDRV_LE_SET(1);
    LEDDRV_LE_SET(0);
    // Turn off LEDs until high side is set (active low output enable)
    LEDDRV_OE_SET(1);
    // Enable the anode high side
    ANO_PORT = ~((column & 0b00011111) | ((column & 0b00100000) << 1));
    // output the LED driver data
    LEDDRV_OE_SET(0);
}


void LEDs_drvr_get_masks(rtc_time_t *t_ptr, uint8_t active_column, uint32_t *row_mask_ptr, uint8_t *col_mask_ptr)
{
    uint8_t col_mask = 0;
    uint16_t row_mask = 0;

    // decide the column mask based on the time & active column
    switch(active_column) {
        case DISP_HOUR_TENS: {
            // find out if this column is active based on the time
            if(t_ptr->hour / 10) {
                col_mask = HOUR_TENS_MASK;
                row_mask = LEDs_drvr_get_row_mask(t_ptr->hour / 10);
            } else {
                col_mask = 0;
            }
            break;
        }

        case DISP_HOUR_ONES: {
            // find out if this column is active based on the time
            if(t_ptr->hour % 10) {
                col_mask = HOUR_ONES_MASK;
                row_mask = LEDs_drvr_get_row_mask(t_ptr->hour % 10);
            } else {
                col_mask = 0;
            }
            break;
        }

        case DISP_MIN_TENS: {
            // find out if this column is active based on the time
            if(t_ptr->min / 10) {
                col_mask = MIN_TENS_MASK;
                row_mask = LEDs_drvr_get_row_mask(t_ptr->min / 10);
            } else {
                col_mask = 0;
            }
            break;
        }

        case DISP_MIN_ONES: {
            // find out if this column is active based on the time
            if(t_ptr->min % 10) {
                col_mask = MIN_ONES_MASK;
                row_mask = LEDs_drvr_get_row_mask(t_ptr->min % 10);
            } else {
                col_mask = 0;
            }
            break;
        }

        case DISP_SEC_TENS: {
            // find out if this column is active based on the time
            if(t_ptr->sec / 10) {
                col_mask = SEC_TENS_MASK;
                row_mask = LEDs_drvr_get_row_mask(t_ptr->sec / 10);
            } else {
                col_mask = 0;
            }
            break;
        }

        case DISP_SEC_ONES: {
            // find out if this column is active based on the time
            if(t_ptr->sec % 10) {
                col_mask = SEC_ONES_MASK;
                row_mask = LEDs_drvr_get_row_mask(t_ptr->sec % 10);
            } else {
                col_mask = 0;
            }
            break;
        }

        default:
            // something went wrong...
        break;
    }
    *row_mask_ptr = row_mask;
    *col_mask_ptr = col_mask;
}

uint32_t LEDs_drvr_get_row_mask(val) {
    switch(val) {
        case 9:
            return (uint32_t)ROW_MASK_NINE;
        case 8:
            return (uint32_t)ROW_MASK_EIGHT;
        case 7:
            return (uint32_t)ROW_MASK_SEVEN;
        case 6:
            return (uint32_t)ROW_MASK_SIX;
        case 5:
            return (uint32_t)ROW_MASK_FIVE;
        case 4:
            return (uint32_t)ROW_MASK_FOUR;
        case 3:
            return (uint32_t)ROW_MASK_THREE;
        case 2:
            return (uint32_t)ROW_MASK_TWO;
        case 1:
            return (uint32_t)ROW_MASK_ONE;
    }
}

