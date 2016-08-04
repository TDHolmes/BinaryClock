#include <avr/io.h>
#include <stdint.h>

int main()
{
    uint32_t i = 0;
    DDRA = (1 << DDA0);
    DDRD = (1 << DDD2);
    while(1) {
        for(i = 0; i < 30000; i++) {
            PORTA = (1 << PA0);
            PORTD = (1 << PD2);
        }
        for(i = 0; i < 30000; i++) {
            PORTA = (0 << PA0);
            PORTD = (0 << PD2);
        }
    }
}
