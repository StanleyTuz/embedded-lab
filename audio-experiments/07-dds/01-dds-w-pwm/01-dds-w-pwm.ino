#include <avr/io.h> // pins, ports
// #include "sine.h"
#include "triangle.h"

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
    uint16_t phase_accumulator = 0;
    uint16_t step_size = 880;
    uint8_t wave_step;
    int8_t pwm_value; 

    init_timer();

    // setup button
    DDRB &= ~(1 << 5);
    PORTB |= (1 << 5);


    while (1) {
        if (bit_is_clear(PINB, 5)) {
            DDRB |= (1 << 7);   // enable output
            phase_accumulator += step_size; // take a step
            wave_step = phase_accumulator >> 8; // take 8 MSBs
            // pwm_value = fullSine[wave_step];    // LUT
            pwm_value = fullTriangle[wave_step];

            loop_until_bit_is_set(TIFR0, TOV0); // Wait for PWM cycle
            OCR0A = 128 + pwm_value;
            TIFR0 |= (1 << TOV0);   // reset PWM overflow bit
        } else {
            DDRB &= ~(1 << 7);  // disable speaker
        }
    }

    return 0;
}
