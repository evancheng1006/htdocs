#include <avr/io.h>
#include <util/delay.h>

int main()
{
    DDRB = 0b11111111;
    while(1)
    {
        PORTB ^= _BV(5);
        _delay_ms(200);
    }
}
