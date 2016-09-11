#ifndef TIMER_4313_H__
#define TIMER_4313_H__

#include <stdint.h>

/*! \file timer_4313.h
    \brief Exposes public timer functions.
*/

void timer_init(volatile uint8_t *counter_var_ptr);
uint32_t timer_millis_get(void);

#endif /* TIMER_4313_H__ */
