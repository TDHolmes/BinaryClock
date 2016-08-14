#include "RTC_DS3231.h"
#include "i2c.c"
#include <stdint.h>


// Summary - 
// param (rtc_time_t *) t_ptr - 
uint8_t RTC_init(rtc_time_t *t_ptr)
{
    uint8_t retval;
    uint8_t byte_to_write = 0;
    // start communication with the chip
    retval = i2c_start((uint8_t)RTC_ADDR, (uint8_t)I2C_MODE_WRITE);
    if (retval != I2C_NO_ERRORS) {
        // handle the error
        return retval;
    }
    // setup the chip (enable 1 Hz output, )
    byte_to_write = (1 << BBSQW);
    i2c_write_byte(RTC_ADDR_CONTROL, byte_to_write);
}


// Summary - 
// param (rtc_time_t *) t_ptr - 
// param (uint8_t) hour - 
// param (uint8_t) minute - 
// param (uint8_t) second - 
uint8_t RTC_set_time(rtc_time_t *t_ptr, uint8_t hour, uint8_t minute, uint8_t second)
{
    uint8_t data_arr[3];
    uint8_t retval;
    // set the time
    data_arr[0] = (second % 10) | ((second / 10) << RTC_SEC_TEN_OFFSET);
    data_arr[1] = (minute % 10) | ((minute / 10) << RTC_MIN_TEN_OFFSET);
    data_arr[2] = (hour % 10) | ((hour / 10) << RTC_HOUR_TEN_OFFSET);
    // send the data to the RTC
    retval = i2c_write((uint8_t)RTC_ADDR_SECONDS, data_arr, 3);
    if (retval == I2C_NO_ERRORS) {
        t_ptr->hour = hour;
        t_ptr->minute = minute;
        t_ptr->second = second;
    }
    return retval;
}


// Summary - 
// param (rtc_time_t *) t_ptr - 
uint8_t RTC_read_time(rtc_time_t *t_ptr)
{
    uint8_t data_arr[3];
    uint8_t val;
    uint8_t retval;
    // send the data to the RTC
    retval = i2c_read((uint8_t)RTC_ADDR_SECONDS, data_arr, 3);
    if (retval != I2C_NO_ERRORS) {
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
