/* Simple example of playing back an audio sample --- stored in 8-bit PCM format
--- via a DAC.

The audio samples are 8 kHz, so I need to set up a timer for that same frequency.
*/
#include <Arduino.h>
#include "PCM_hello.h"


#define PORT_OUT PORTA
#define DDR_OUT DDRA


const uint16_t table_length = sizeof(PCM_hello);
volatile uint16_t sample_index = 0; // where we are in the sample

bool do_sample = false;
uint8_t sample = 128;

void setup_sample_timer() {
    /* System clock is 16 MHz, sample/interrupt rate is 8 kHz.
    => interrupt every 2,000 timer ticks (16-bit timer 1).
    */
    TCCR1A = 0;
    TCCR1B = 0;

    // Set CTC mode (WGM)
    TCCR1B |= (1 << WGM12);
    // Not using output pin, so no Output Mode settings

    // Enable interrupts for Compare Match
    TIMSK1 |= (1 << OCIE1A);
    
    // Set sample rate to 8 kHz (2000 ticks)
    OCR1A = 3000-1;

    // Set prescaler = 1
    TCCR1B |= (1 << CS10);
}

ISR(TIMER1_COMPA_vect) {
    do_sample = true;
}

uint8_t get_sample() {
    /* Read a sample byte (8-bit value) from Flash memory. */
    uint8_t sample = pgm_read_byte(&PCM_hello[sample_index]);
    
    sample_index++;
    if (sample_index >= table_length) {
        // wrap back around
        sample_index = 0;
    }
    return sample;
}

void setup() {

    setup_sample_timer();
    sei();

    // Set output pins to OUTPUT mode
    DDR_OUT = 0xFFFF;
}

void loop() {
    if (do_sample) {
        do_sample = false;

        // get sample from Flash memory
        sample = get_sample();

        PORT_OUT = sample;
    }
}