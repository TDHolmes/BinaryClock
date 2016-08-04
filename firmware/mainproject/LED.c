#include "LED.h"
#include "timer.h"
#include "ports.h"
#include "LED_drvr_STP16CPC26.h"
#include <stdint.h>

LED_drvr_t LED_admin_struct;
LED_drvr_t *LED_admin_ptr;

void LED_init(void)
{
    LED_admin_ptr = &LED_admin_struct;
    LED_drvr_init(LED_admin_ptr);
    LED_clear_all();
}

// update LED based on a time structure
void LED_update_time(rtc_time_t *t_ptr)
{
    // check if we should only update the changed values
    uint8_t i;
    uint8_t hour_ten = t_ptr->hour / 10;
    uint8_t hour_one = t_ptr->hour % 10;
    uint8_t min_ten = t_ptr->min / 10;
    uint8_t min_one = t_ptr->min % 10;
    uint8_t sec_ten = t_ptr->sec / 10;
    uint8_t sec_one = t_ptr->sec % 10;
    for (i = 0; i < 4; i++) {
        // if the hour is changed...
        if (t_ptr->min == 0) {
            // update the i'th bit of hour tens place
            if (hour_ten & (1 << i)) {
                LED_admin_ptr->LED_array[i][0][RED_IND] = LED_admin_ptr->colors.red;
                LED_admin_ptr->LED_array[i][0][GREEN_IND] = LED_admin_ptr->colors.green;
                LED_admin_ptr->LED_array[i][0][BLUE_IND] = LED_admin_ptr->colors.blue;
            } else {
                LED_admin_ptr->LED_array[i][0][RED_IND] = 0;
                LED_admin_ptr->LED_array[i][0][GREEN_IND] = 0;
                LED_admin_ptr->LED_array[i][0][BLUE_IND] = 0;
            }
            // update the i'th bit of hour ones place
            if (hour_one & (1 << i)) {
                LED_admin_ptr->LED_array[i][1][RED_IND] = LED_admin_ptr->colors.red;
                LED_admin_ptr->LED_array[i][1][GREEN_IND] = LED_admin_ptr->colors.green;
                LED_admin_ptr->LED_array[i][1][BLUE_IND] = LED_admin_ptr->colors.blue;
            } else {
                LED_admin_ptr->LED_array[i][1][RED_IND] = 0;
                LED_admin_ptr->LED_array[i][1][GREEN_IND] = 0;
                LED_admin_ptr->LED_array[i][1][BLUE_IND] = 0;
            }
        }
        // if minutes have changed...
        if (t_ptr->sec == 0) {
            // update the i'th bit of min tens place
            if (min_ten & (1 << i)) {
                LED_admin_ptr->LED_array[i][2][RED_IND] = LED_admin_ptr->colors.red;
                LED_admin_ptr->LED_array[i][2][GREEN_IND] = LED_admin_ptr->colors.green;
                LED_admin_ptr->LED_array[i][2][BLUE_IND] = LED_admin_ptr->colors.blue;
            } else {
                LED_admin_ptr->LED_array[i][2][RED_IND] = 0;
                LED_admin_ptr->LED_array[i][2][GREEN_IND] = 0;
                LED_admin_ptr->LED_array[i][2][BLUE_IND] = 0;
            }
            // update the i'th bit of min ones place
            if (min_one & (1 << i)) {
                LED_admin_ptr->LED_array[i][3][RED_IND] = LED_admin_ptr->colors.red;
                LED_admin_ptr->LED_array[i][3][GREEN_IND] = LED_admin_ptr->colors.green;
                LED_admin_ptr->LED_array[i][3][BLUE_IND] = LED_admin_ptr->colors.blue;
            } else {
                LED_admin_ptr->LED_array[i][3][RED_IND] = 0;
                LED_admin_ptr->LED_array[i][3][GREEN_IND] = 0;
                LED_admin_ptr->LED_array[i][3][BLUE_IND] = 0;
            }
        }
        // just always update the seconds
        // update the i'th bit of sec tens place
        if (sec_ten & (1 << i)) {
            LED_admin_ptr->LED_array[i][4][RED_IND] = LED_admin_ptr->colors.red;
            LED_admin_ptr->LED_array[i][4][GREEN_IND] = LED_admin_ptr->colors.green;
            LED_admin_ptr->LED_array[i][4][BLUE_IND] = LED_admin_ptr->colors.blue;
        } else {
            LED_admin_ptr->LED_array[i][4][RED_IND] = 0;
            LED_admin_ptr->LED_array[i][4][GREEN_IND] = 0;
            LED_admin_ptr->LED_array[i][4][BLUE_IND] = 0;
        }
        // update the i'th bit of sec ones place
        if (sec_one & (1 << i)) {
            LED_admin_ptr->LED_array[i][5][RED_IND] = LED_admin_ptr->colors.red;
            LED_admin_ptr->LED_array[i][5][GREEN_IND] = LED_admin_ptr->colors.green;
            LED_admin_ptr->LED_array[i][5][BLUE_IND] = LED_admin_ptr->colors.blue;
        } else {
            LED_admin_ptr->LED_array[i][5][RED_IND] = 0;
            LED_admin_ptr->LED_array[i][5][GREEN_IND] = 0;
            LED_admin_ptr->LED_array[i][5][BLUE_IND] = 0;
        }
    }
}

// called cyclicly in while loop
void LED_run(uint8_t LED_multiplex_timer_count)
{
    LED_drvr_run(LED_admin_ptr, LED_multiplex_timer_count);
}


// clears the LED at the given index
void LED_clear(uint8_t row, uint8_t col)
{
    LED_admin_ptr->LED_array[row][col][RED_IND] = 0;
    LED_admin_ptr->LED_array[row][col][GREEN_IND] = 0;
    LED_admin_ptr->LED_array[row][col][BLUE_IND] = 0;
}

// sets the LED at the given index to the given color struct
void LED_set(uint8_t row, uint8_t col, uint8_t red, uint8_t green, uint8_t blue)
{
    LED_admin_ptr->LED_array[row][col][RED_IND] = red;
    LED_admin_ptr->LED_array[row][col][GREEN_IND] = green;
    LED_admin_ptr->LED_array[row][col][BLUE_IND] = blue;
}

// clears all LED
void LED_clear_all(void)
{
    int i, j, k;
    for (i = 0; i < ROW_SIZE; i++) {
        for (j = 0; j < COL_SIZE; j++) {
            for (k = 0; k < COLOR_SIZE; k++) {
                LED_admin_ptr->LED_array[i][j][k] = 0;
            }
        }
    }
}

// sets all LEDs to the given color
void LED_set_all(uint8_t red, uint8_t blue, uint8_t green)
{
    int i, j;
    for (i = 0; i < ROW_SIZE; i++) {
        for (j = 0; j < COL_SIZE; j++) {
            LED_admin_ptr->LED_array[i][j][RED_IND] = red;
            LED_admin_ptr->LED_array[i][j][GREEN_IND] = green;
            LED_admin_ptr->LED_array[i][j][BLUE_IND] = blue;
        }
    }
}

void LED_set_color(uint8_t red, uint8_t blue, uint8_t green)
{
    LED_admin_ptr->colors.red = red;
    LED_admin_ptr->colors.green = green;
    LED_admin_ptr->colors.blue = blue;
}


// // returns the accrued errors
// void LED_errors_check(LED_errors_t LED_error_struct_ptr)
// {

// }

// // resets any errors
// void LED_errors_reset()
// {

// }
