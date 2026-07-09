/* Output a 440 Hz sine wave via DAC. */

#include <Arduino.h>
#include "sine.h"
#include "triangle.h"

#define F_SAMPLE 20000
#define F_OUT 440
#define PORT_OUT PORTA
#define DDR_OUT DDRA

#define TEST_PIN 5

// volatile bool do_sample = false;
uint16_t phase_accumulator = 0;
uint16_t phase_increment = (uint32_t)F_OUT * 65536UL / F_SAMPLE;
uint8_t phase_index;
uint8_t value;

/* Configure the 16-bit Timer1.

    To sample rate F_SAMPLE, we need to interrupt every
    F_CPU / F_SAMPLE ticks.
*/
void configure_timer1() {

    TCCR1A = 0;
    TCCR1B = 0;
    
    // Set WGM to CTC
    TCCR1B |= (1 << WGM12);
    // Keep Output Mode normal (not using output pin)

    // Set the prescaler to 1
    TCCR1B |= (1 << CS10);

    // Set the overflow compare register
    OCR1A = 800; // F_CPU / F_SAMPLE;

    // Enable the interrupts on this timer
    TIMSK1 |= (1 << OCIE1A);

    sei();
}

ISR(TIMER1_COMPA_vect) {
    // debug pin
    // do_sample = true;

    // // Take a sample
    sample_table();
    
    // Output the current values
    output_to_dac();
}

void setup() {
    // Serial.begin(115200);

    // Set output pins to output mode
    DDR_OUT = 0xFFFF;
    
    pinMode(TEST_PIN, OUTPUT);

    configure_timer1();

    phase_accumulator = 0;
}


void sample_table() {
    // Take a sample
    phase_accumulator += phase_increment;
    // Shift to get the index
    phase_index = (phase_accumulator >> 8);
    // Index into the LUT
    value = 128 + fullSine[phase_index];
    // value = (phase_index < 128) ? 255 : 0;
}

void output_to_dac() {
    // Write the output value to the port
    PORT_OUT = value;
}

void loop() {
    // if (do_sample) {
        // do_sample = false;

        // // Take a sample
        // sample_table();
        
        // // Output the current values
        // output_to_dac();

        // Serial.println(value);
    // }
}