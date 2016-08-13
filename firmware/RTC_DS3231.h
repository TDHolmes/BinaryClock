#ifndef RTC_DS3231_H__
#define RTC_DS3231_H__

#include "timer.h"

#define RTC_ADDR 0x68

/**** address definitions ****/
#define RTC_ADDR_SECONDS           0x00
#define RTC_ADDR_MINUTES           0x01
#define RTC_ADDR_HOURS             0x02
#define RTC_ADDR_DAY               0x03
#define RTC_ADDR_DATE              0x04
#define RTC_ADDR_MONTH_CENTURY     0x05
#define RTC_ADDR_YEAR              0x06
#define RTC_ADDR_ALARM_1_SECONDS   0x07
#define RTC_ADDR_ALARM_1_MINUTES   0x08
#define RTC_ADDR_ALARM_1_HOURS     0x09
#define RTC_ADDR_ALARM_1_DAY_DATE  0x0A
#define RTC_ADDR_ALARM_2_MINUTES   0x0B
#define RTC_ADDR_ALARM_2_HOURS     0x0C
#define RTC_ADDR_ALARM_2_DAY_DATE  0x0D
#define RTC_ADDR_CONTROL           0x0E
#define RTC_ADDR_STATUS            0x0F
#define RTC_ADDR_AGING_OFFSET      0x10
#define RTC_ADDR_TEMP_MSB          0x11
#define RTC_ADDR_TEMP_LSB          0x12

/**** bit definitions ****/
// control reg bits
#define EOSC_N 7
#define BBSQW  6
#define CONV   5
#define INTCN  2
#define A2IE   1
#define A1IE   0

// status reg bits
#define OSF     7
#define EN32KHZ 3
#define BSY     2
#define A2F     1
#define A1F     0


/**** Masks ****/
#define RTC_SEC_ONE_MASK  0b00001111
#define RTC_SEC_TEN_MASK  0b01110000
#define RTC_MIN_ONE_MASK  0b00001111
#define RTC_MIN_TEN_MASK  0b01110000
#define RTC_HOUR_ONE_MASK 0b00001111
#define RTC_HOUR_TEN_MASK 0b00110000

// offsets
#define RTC_SEC_TEN_OFFSET  4
#define RTC_MIN_TEN_OFFSET  4
#define RTC_HOUR_TEN_OFFSET 4


/* The Aging Offset code is encoded in two’s complement, with bit 7 representing the
SIGN bit and a valid range of ±127. One LSB typically represents a 0.12ppm change in
frequency. The change in ppm per LSB is the same over the operating temperature range.
Positive offsets slow the time base and negative offsets quicken the time base. */

#endif /* RTC_DS3231_H__ */
