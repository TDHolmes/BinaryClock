#ifndef TIMER_DRVR_4313_H__
#define TIMER_DRVR_4313_H__

#include <stdint.h>

void timer_counter_drvr_init(uint8_t *counter_var_ptr);
void timer_millis_drvr_init(void);
uint32_t timer_millis_drvr_get(void);

#endif /* TIMER_DRVR_4313_H__ */
