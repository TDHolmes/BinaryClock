#include "timer_drvr_4313.h"


// instantiate & set defaults
timer_drvr_status_t timer_drvr_status;
timer_drvr_status_t *t_status_ptr = &timer_drvr_status;

t_status_ptr->millis_initialized = 0;
t_status_ptr->counter_initialized = 0;
t_status_ptr->counter_flag_ptr = 0;
t_status_ptr->millis_count = 0;

void timer_counter_drvr_init(uint8_t *counter_var_ptr)
{
    // this timer will use the 8 bit timer
    t_status_ptr->counter_flag_ptr = counter_flag_ptr;
    // for a multiplexer updating for 4 bit color,
    // f_osc_timer = (8 MHz)/(2 * 8 * (1 + 173)) = 2.874 kHz 
    //    where 8 is the multiplier, 173 is the target number
    TCCR0A = 0b00000000;  // normal modes
    TCCR0B = 0b00000010;  // clock source = 010 for fosc / 8
    OCR0A = 173;  // output compare to 173 to generate interrupts at 2.874 kHz
    TIMSK = 1;    // enable an interrupt on OCR0A compare match
    t_status_ptr->counter_initialized = 1;
}


void timer_millis_drvr_init(void)
{
    t_status_ptr->millis_count = 0;
    t_status_ptr->millis_initialized = 1;
}


uint32_t timer_millis_drvr_get(void)
{

}


// Interrupt service routine on comp A match
ISR(TIMER0_COMPA_vect)
{
    TCNT0 = 0;  // reset the timer
    *(t_status_ptr->counter_flag_ptr) += 1;
}
