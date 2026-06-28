#include <avr/io.h> // pins, ports
#include "sine.h"



void init_timer() {
    // Set up Timer 0 (8-bit, with PWM support)
    // Set to Fast PWM mode
    TCCR0A |= (1 << WGM00) | (1 << WGM01);

    // Set prescaler
    TCCR0B |= (1 << CS00);

    // Enable direct PWM output to pin (connect timer output to the pin PB7/~D13)
    // This means that PORTB will no longer control that pin!
    TCCR0A |= (1 << COM0A1);
    // Enable output on pin PB7
    DDRB |= (1 << 7);
}

int main() {

    // ----- Setup ------ //
    uint16_t phase_accumulator;
    uint16_t step_size;
    uint8_t wave_step;
    int8_t pwm_value; 

    init_timer();


    while (1) {

    }

    return 0;
}
