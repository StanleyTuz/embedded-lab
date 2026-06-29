/* Create a perfectly stable 1 Hz tick using CTC mode and interrupt.

This is a very low frequency, so I will need to either use a wide timer
or do some counting in software. Timer 1 is 16-bit, so I'll try that one.

f_out = 1 Hz = (16 MHz) / (2 * p * (1 + OCR))
<=>
p * ( 1 + OCR ) = 8 M
If we try p = 1024, we get OCR = 7812.5, which we can achieve in 16 bits. 
*/

#include <avr/io.h>

void setup_timer1() {
    TCCR1A = 0;
    TCCR1B = 0;
    // Set WGM:
    // I want CTC mode: WGMn2 = 1 which is register in TCCR1B
    // In this mode, TOP = OCRnA
    TCCR1B |= (1 << WGM12); 

    // Set Output mode:
    // I want to toggle the output pin (OC1A) on a compare match
    TCCR1A |= (1 << COM1A0);

    // Set prescaler to 1024
    TCCR1B |= (1 << CS12) | (1 << CS10);

    // Set compare match
    OCR1A = 7813 - 1;
}

int main() {

    // Setup
    setup_timer1();
    DDRB |= (1 << PB5);

    while (1) {

    }

    return 0;
}