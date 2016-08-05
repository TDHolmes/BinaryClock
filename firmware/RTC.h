#ifndef RTC_H__
#define RTC_H__

#include "timer.h"

void RTC_set_time(rtc_time_t *t_ptr, uint8_t hour, uint8_t minute, uint8_t second);

#endif /* RTC_H__ */
