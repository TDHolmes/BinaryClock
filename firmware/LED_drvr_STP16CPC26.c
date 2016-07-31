#include "LED_drvr_STP16CPC26.h"
#include "LED.h"
#include "ports.h"
#include <stdint.h>


void LED_drvr_init(LED_drvr_t * LED_ptr)
{
    LED_ptr->LED_errors.missed_multiplex_timer_interrupts = 0;
    LED_ptr->active_column = 0;
    LED_ptr->active_color_bit = 0;
    LED_drvr_clear_all(LED_ptr); // clear the LED array
}


void LED_drvr_clear_all(LED_drvr_t * LED_ptr)
{
    uint8_t i, j, k;
    // clear the LED Array
    for(i = 0; i < ROWS; i++) {
        for(j = 0; j < COLS; j++) {
            for(k = 0; k < 3; k++) {
                LED_ptr->LED_array[i][j][k] = 0;
            }
        }
    }
}


void LED_drvr_run(LED_drvr_t *LED_ptr, uint8_t LED_multiplex_timer_count)
{
    uint8_t color_bit = LED_ptr->active_color_bit;
    uint8_t color_mask = (1 << color_bit);
    uint8_t active_column = LED_ptr->active_column;
    uint16_t bitstream = 0;
    uint8_t bit = 0;
    uint8_t column_mask = 0;
    uint8_t row, color;

    // if we missed some multiplex timer counts, increment the errors
    if (LED_multiplex_timer_count != 1) {
        LED_ptr->LED_errors->missed_multiplex_timer_interrupts += (LED_multiplex_timer_count - 1);
    }
    // build up the bitstream to be clocked out
    for (row = 0; row < 4; row++) { // rows
        for(color = 0; color < 3; color++) { // colors
            bit = ((LED_ptr->LED_array[row][active_column][color] & color_mask) >> color_bit);
            bitstream |= (bit << (row * 3 + color));
        }
    }
    // increment the variables in the LED_ptr struct
    if (LED_ptr->active_column == 5) {
        LED_ptr->active_column += 1;
    } else {
        LED_ptr->active_column = 0;
        // update the color bit too
        if (LED_ptr->active_color_bit < 7) {
            LED_ptr->active_color_bit += 1;
        } else {
            LED_ptr->active_color_bit = 0;
        }
    }
    // clock out the LED bits
    for(i = 0; i < 12; i++) {
        LEDDRV_CLK_SET(0);
        LEDDRV_SDI_SET((bitstream >> i) & 0x0001);
        LEDDRV_CLK_SET(1);
    }
    // clock out the rest in zeros
    for(i = 0; i < 8; i++) {
        LEDDRV_CLK_SET(0);
        LEDDRV_SDI_SET(0);
        LEDDRV_CLK_SET(1);
    }
    // Turn off LED until everything is set (OE is active low)
    LEDDRV_OE_SET(1);
    // Latch the LED driver data
    LEDDRV_LE_SET(1);
    LEDDRV_LE_SET(0);
    // Enable the anode high side
    //ANO_PORT = // some mask...  // <--- FIXME
    // output the LED driver data
    LEDDRV_OE_SET(0);
}

// void LED_drvr_update(uint16_t row_mask, uint8_t active_column)
// {
//     // apply the color & time masks to the active rows
//     row = color_mask & row_mask;
//     // clock out data to LED driver
//     // unroll loop if that's a pragma option
//     for(bits = 20; bits != 0; bits -= 1) {
//         LEDDRV_CLK_SET(0);
//         LEDDRV_SDI_SET(row & 0x01);
//         row = row >> 1;
//         LEDDRV_CLK_SET(1);
//     }
//     // Latch the LED driver data
//     LEDDRV_LE_SET(1);
//     LEDDRV_LE_SET(0);
//     // Turn off LED until high side is set (active low output enable)
//     LEDDRV_OE_SET(1);
//     // Enable the anode high side
//     ANO_PORT = ~((column & 0b00011111) | ((column & 0b00100000) << 1));
//     // output the LED driver data
//     LEDDRV_OE_SET(0);
// }


// void LED_drvr_get_masks(rtc_time_t *t_ptr, uint8_t active_column, uint32_t *row_mask_ptr, uint8_t *col_mask_ptr)
// {
//     uint8_t col_mask = 0;
//     uint16_t row_mask = 0;

//     // decide the column mask based on the time & active column
//     switch(active_column) {
//         case DISP_HOUR_TENS: {
//             // find out if this column is active based on the time
//             if(t_ptr->hour / 10) {
//                 col_mask = HOUR_TENS_MASK;
//                 row_mask = LED_drvr_get_row_mask(t_ptr->hour / 10);
//             } else {
//                 col_mask = 0;
//             }
//             break;
//         }

//         case DISP_HOUR_ONES: {
//             // find out if this column is active based on the time
//             if(t_ptr->hour % 10) {
//                 col_mask = HOUR_ONES_MASK;
//                 row_mask = LED_drvr_get_row_mask(t_ptr->hour % 10);
//             } else {
//                 col_mask = 0;
//             }
//             break;
//         }

//         case DISP_MIN_TENS: {
//             // find out if this column is active based on the time
//             if(t_ptr->min / 10) {
//                 col_mask = MIN_TENS_MASK;
//                 row_mask = LED_drvr_get_row_mask(t_ptr->min / 10);
//             } else {
//                 col_mask = 0;
//             }
//             break;
//         }

//         case DISP_MIN_ONES: {
//             // find out if this column is active based on the time
//             if(t_ptr->min % 10) {
//                 col_mask = MIN_ONES_MASK;
//                 row_mask = LED_drvr_get_row_mask(t_ptr->min % 10);
//             } else {
//                 col_mask = 0;
//             }
//             break;
//         }

//         case DISP_SEC_TENS: {
//             // find out if this column is active based on the time
//             if(t_ptr->sec / 10) {
//                 col_mask = SEC_TENS_MASK;
//                 row_mask = LED_drvr_get_row_mask(t_ptr->sec / 10);
//             } else {
//                 col_mask = 0;
//             }
//             break;
//         }

//         case DISP_SEC_ONES: {
//             // find out if this column is active based on the time
//             if(t_ptr->sec % 10) {
//                 col_mask = SEC_ONES_MASK;
//                 row_mask = LED_drvr_get_row_mask(t_ptr->sec % 10);
//             } else {
//                 col_mask = 0;
//             }
//             break;
//         }

//         default:
//             // something went wrong...
//         break;
//     }
//     *row_mask_ptr = row_mask;
//     *col_mask_ptr = col_mask;
// }

// uint32_t LED_drvr_get_row_mask(val) {
//     switch(val) {
//         case 9:
//             return (uint32_t)ROW_MASK_NINE;
//         case 8:
//             return (uint32_t)ROW_MASK_EIGHT;
//         case 7:
//             return (uint32_t)ROW_MASK_SEVEN;
//         case 6:
//             return (uint32_t)ROW_MASK_SIX;
//         case 5:
//             return (uint32_t)ROW_MASK_FIVE;
//         case 4:
//             return (uint32_t)ROW_MASK_FOUR;
//         case 3:
//             return (uint32_t)ROW_MASK_THREE;
//         case 2:
//             return (uint32_t)ROW_MASK_TWO;
//         case 1:
//             return (uint32_t)ROW_MASK_ONE;
//     }
// }

