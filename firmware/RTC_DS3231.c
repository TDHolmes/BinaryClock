#include "RTC_DS3231.h"
#include "i2c.h"
#include "global_defines.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

/*! \file RTC_DS3231.c
    \brief Functions for interfacing with the Maxim DS3231 real time clock chip.
*/


/*! pointer to the variable in main that keeps track of the RTC interrupt
 *  count for accurate time keeping.  */
volatile uint8_t *RTC_local_1Hz_int_count_ptr;


/*!
 * Real time clock (RTC) initialization function that sets up the 1 Hz pin change interrupt
 * and the I2C communication to the chip to configure it and read the time it has internally.
 * 
 * @param[in]  time_ptr (rtc_time_t *): Pointer that is used to keep track of the time.
 * @param[in]  RTC_int_cnt_ptr (volatile uint8_t *): RTC interrupt count pointer
 *             so we can keep track of the interrupt count from outside of this file.
 * @param[out] retval (retval_t): Returns the initialization status of the RTC chip.
 */
retval_t RTC_init(rtc_time_t *t_ptr, volatile uint8_t *RTC_int_cnt_ptr)
{
    uint8_t byte_to_write = 0;
    retval_t retval = 0;
    RTC_local_1Hz_int_count_ptr = RTC_int_cnt_ptr;
    // enable a pin change interrupt on the 1 Hz RTC pin 
    GIMSK |= (1 << PCIE1);
    PCMSK1 = (1 << PCINT9);
    // start communication with the chip & setup the chip (enable 1 Hz output, )
    byte_to_write = (1 << BBSQW);
    retval = i2c_write_byte(RTC_ADDR, RTC_ADDR_CONTROL, byte_to_write);
    if (retval != GEN_PASS) {
        return retval;
    }
    retval = RTC_read_time(t_ptr);
    return retval;
}


/*!
 * Updates the time on the RTC chip as well as in the time keeping data structure t_ptr.
 * 
 * @param[in] t_ptr (rtc_time_t *): Pointer that is used to keep track of the time.
 * @param[in] hour (uint8_t): hour to change the time to on the RTC chip as wel as in
 *      the time pointer.
 * @param[in] minute (uint8_t): minute to change the time to on the RTC chip as wel as
 *      in the time pointer.
 * @param[in] second (uint8_t): second to change the time to on the RTC chip as wel as
 *      in the time pointer.
 * @param[out] retval (retval_t): Returns the success or failure of updating the time on
 *      the RTC chip.
 */
retval_t RTC_set_time(rtc_time_t *t_ptr, uint8_t hour, uint8_t minute, uint8_t second)
{
    uint8_t data_arr[3];
    retval_t retval;
    // set the time
    data_arr[0] = (second % 10) | ((second / 10) << RTC_SEC_TEN_OFFSET);
    data_arr[1] = (minute % 10) | ((minute / 10) << RTC_MIN_TEN_OFFSET);
    data_arr[2] = (hour   % 10) | ((hour / 10) << RTC_HOUR_TEN_OFFSET);
    // send the data to the RTC
    retval = i2c_write(RTC_ADDR, (uint8_t)RTC_ADDR_SECONDS, data_arr, 3);
    if (retval == GEN_PASS) {
        t_ptr->hour = hour;
        t_ptr->minute = minute;
        t_ptr->second = second;
    }
    return retval;
}


/*!
 * Reads the time on the RTC chip as well as updating the time keeping data structure t_ptr.
 * 
 * @param[in] t_ptr (rtc_time_t *): Pointer that is used to keep track of the time.
 * @param[out] retval (retval_t): Returns the success or failure of reading the time from
 *      the RTC chip.
 */
retval_t RTC_read_time(rtc_time_t *t_ptr)
{
    uint8_t data_arr[3];
    uint8_t val;
    retval_t retval;
    // send the data to the RTC
    retval = i2c_read(RTC_ADDR, (uint8_t)RTC_ADDR_SECONDS, data_arr, 3);
    if (retval != GEN_PASS) {
        // some sort of error
        return retval;
    }
    // decrypt the time
    val = data_arr[0];
    t_ptr->second = (val & RTC_SEC_ONE_MASK) + 10 * ((val & RTC_SEC_TEN_MASK) >> RTC_SEC_TEN_OFFSET);
    val = data_arr[1];
    t_ptr->minute = (val & RTC_MIN_ONE_MASK) + 10 * ((val & RTC_MIN_TEN_MASK) >> RTC_MIN_TEN_OFFSET);
    val = data_arr[2];
    t_ptr->hour = (val & RTC_HOUR_ONE_MASK) + 10 * ((val & RTC_HOUR_TEN_MASK) >> RTC_HOUR_TEN_OFFSET);
    return retval;
}


/*!
 * Interrupts on pin change form the RTC chip which is a 1 Hz square wave.
 * Increments the counter to keep the time up to date.
 */
ISR(PCINT_A_vect)
{
    if (PINA & (1 << PA1)) {
        *RTC_local_1Hz_int_count_ptr += 1;
    }
}
