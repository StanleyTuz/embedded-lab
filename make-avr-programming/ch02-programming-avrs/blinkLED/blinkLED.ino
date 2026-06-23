// ----- Preamble ----- //
#include <avr/io.h>     /* Defines pins, ports, etc. */
#include <util/delay.h> /* Functions to waste time. */

int main(void) {

    // ----- Inits ----- //
    // Port B Data Direction Register.
    DDRB |= 0b10000000; // Enabling output for pin PB7
    // or
    // DDRB |= (1 << PB7);


    // ----- Event loop ----- //
    while (1) {
        // Port B Data Register.
        PORTB = 0b10000000;    // Turn on 7th pin in PORTB
        _delay_ms(500);
        
        PORTB = 0b00000000;    // Turn off 7th pin in PORTB
        _delay_ms(500);
    }

    return 0;
}

