#include "timer_4313.h"
#include <avr/interrupt.h>


typedef struct {
    uint8_t millis_initialized;
    uint8_t micros_initialized;
    uint8_t counter_initialized;
    uint8_t *counter_flag_ptr;
    uint32_t millis_count;
    uint32_t micros_count;
} timer_status_t;


// instantiate & set defaults
timer_status_t timer_status;
timer_status_t *t_status_ptr;



// private function definitions
void timer_counter_init(uint8_t *counter_var_ptr);
void timer_millis_init(void);


void timer_init(uint8_t *counter_var_ptr)
{
    t_status_ptr = &timer_status;
    t_status_ptr->millis_initialized = 0;
    t_status_ptr->counter_initialized = 0;
    t_status_ptr->counter_flag_ptr = 0;
    t_status_ptr->millis_count = 0;
    timer_counter_init(counter_var_ptr);
    timer_millis_init();
}


// Summary - 
// param (uint8_t *) counter_var_ptr - 
void timer_counter_init(uint8_t *counter_var_ptr)
{
    // this timer will use the 8 bit timer
    t_status_ptr->counter_flag_ptr = counter_var_ptr;
    // for a multiplexer updating for 4 bit color,
    // f_osc_timer = (8 MHz)/(2 * 8 * (1 + 173)) = 2.874 kHz 
    //    where 8 is the multiplier, 173 is the target number
    TCCR0A = (1 << WGM01);   // CTC mode (clear timer on compare match)
    TCCR0B = (1 << CS01);    // set CS01 for fosc / 8 prescale
    OCR0A = 173;             // output compare to 173 to generate interrupts at 2.874 kHz
    TCNT0 = 0;               // reset timer count
    TIMSK |= (1 << OCIE0A);  // enable timer
    t_status_ptr->counter_initialized = 1;
}


// Summary - 
void timer_millis_init(void)
{
    // update at 1kHz
    TCCR1B = 0b00001010;  // CTC mode, clock div 8 mode
    // TCCR1B = (1 << X | 1 << Y);
    OCR1A = 999;          // (8 MHz) / (8 * (1 + 999)) = 1kHz
    TCNT1 = 0;            // reset the timer
    TIMSK |= (1 << OCIE1A);        // enable an interrupt on OCR1A compare match
    t_status_ptr->millis_count = 0;
    t_status_ptr->millis_initialized = 1;
}


// Summary - 
// retval (uint32_t) - 
uint32_t timer_millis_get(void)
{
    return t_status_ptr->millis_count;
}


// Interrupt service routine on comp A match
// Summary - 
ISR(TIMER1_COMPA_vect)
{
    cli();
    TCNT1 = 0;  // reset the timer
    OCR1A = 999;  // (8 MHz) / (8 * (1 + 9)) = 1 kHz
    t_status_ptr->millis_count += 1;
    sei();
}


// Interrupt service routine on comp A match
// Summary - 
ISR(TIMER0_COMPA_vect)
{
    cli();
    TCNT0 = 0;        // reset the timer
    OCR0A = 173;      // output compare to 173 to generate interrupts at 2.874 kHz
    *(t_status_ptr->counter_flag_ptr) += 1;
    sei();
}
