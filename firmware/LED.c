#include "LED.h"
#include "color.h"
#include "ports.h"
#include "RTC_DS3231.h"
#include "LED_drvr_STP16CPC26.h"
#include <stdint.h>

LED_drvr_t LED_admin_struct;  //!< Structure that keeps track of important LED variables.
LED_drvr_t *LED_admin_ptr;    //!< Pointer to the LED admin structure.


/*!
 * initializes the LED driver, clears the LEDs, and initializes
 * the LED admin pointer.
 */
void LED_init(void)
{
    // uint8_t i;
    LED_admin_ptr = &LED_admin_struct;
    LED_drvr_init(LED_admin_ptr);
    LED_clear_all();
}


/*!
 * Updates the time on the RTC chip as well as in the time keeping data structure t_ptr.
 * 
 * @param[in] t_ptr (rtc_time_t *): Pointer that is used to keep track of the time.
 * @param[in] update_all (uint8_t): Determines if we should update all LEDs or only
 *      the ones that should have changed second to second.
 */
void LED_update_time(rtc_time_t *t_ptr, uint8_t update_all)
{
    // check if we should only update the changed values
    uint8_t i;
    uint8_t hour_ten = t_ptr->hour / 10;
    uint8_t hour_one = t_ptr->hour % 10;
    uint8_t min_ten = t_ptr->minute / 10;
    uint8_t min_one = t_ptr->minute % 10;
    uint8_t sec_ten = t_ptr->second / 10;
    uint8_t sec_one = t_ptr->second % 10;
    for (i = 0; i < 4; i++) {
        // if the hour is changed...
        if ((t_ptr->minute == 0) | (update_all != 0)) {
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
        if ((t_ptr->second == 0) | (update_all != 0)) {
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


/*!
 * Runs the LED driver run function. Used for multiplexing.
 */
void LED_run(void)
{
    LED_drvr_run(LED_admin_ptr);
}


/*!
 * Clears an LED at the given row & column.
 * 
 * @param[in] row (uint8_t): row of the LED to clear.
 * @param[in] col (uint8_t): column of the LED to clear.
 */
void LED_clear(uint8_t row, uint8_t col)
{
    LED_admin_ptr->LED_array[row][col][RED_IND] = 0;
    LED_admin_ptr->LED_array[row][col][GREEN_IND] = 0;
    LED_admin_ptr->LED_array[row][col][BLUE_IND] = 0;
}


/*!
 * sets the LED at the given index to the given color struct
 * 
 * @param[in] row  (uint8_t): row of the LED to set.
 * @param[in] col  (uint8_t): column of the LED to set.
 * @param[in] red  (uint8_t): amount of red color to set the LED to.
 * @param[in] green (uint8_t): amount of green color to set the LED to.
 * @param[in] blue  (uint8_t): amount of blue color to set the LED to.
 */
void LED_set(uint8_t row, uint8_t col, uint8_t red, uint8_t green, uint8_t blue)
{
    LED_admin_ptr->LED_array[row][col][RED_IND] = red;
    LED_admin_ptr->LED_array[row][col][GREEN_IND] = green;
    LED_admin_ptr->LED_array[row][col][BLUE_IND] = blue;
}

/*!
 * Clears all of the LEDs.
 */
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


/*!
 * sets all of the LEDs to the given color.
 * 
 * @param[in] red  (uint8_t): amount of red color to set all the LEDs to.
 * @param[in] green (uint8_t): amount of green color to set all the LEDs to.
 * @param[in] blue  (uint8_t): amount of blue color to set all the LEDs to.
 */
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


/*!
 * sets all of the active time LEDs to the given color.
 * 
 * @param[in] red  (uint8_t): amount of red color to set all the active time LEDs to.
 * @param[in] green (uint8_t): amount of green color to set all the active time LEDs to.
 * @param[in] blue  (uint8_t): amount of blue color to set all the active time LEDs to.
 */
void LED_set_color(uint8_t red, uint8_t blue, uint8_t green)
{
    LED_admin_ptr->colors.red = red;
    LED_admin_ptr->colors.green = green;
    LED_admin_ptr->colors.blue = blue;
}
