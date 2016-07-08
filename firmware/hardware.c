#include "hardware.h"

void LED0_set(uint8_t val)
{
    if(val == 1) {
        PORTA |= PORTA0;
    } else {
        PORTA &= PORTA0;
    }
}

void LED0_set(uint8_t val)
{
    if(val == 1) {
        PORTD |= PORTD2;
    } else {
        PORTD &= PORTD2;
    }
}
