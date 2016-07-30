#ifndef TIMER_H__
#define TIMER_H__

#include <stdint.h>

typedef struct {
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
} rtc_time_t;

void reset_timer(void);

#endif /* TIMER_H__ */
