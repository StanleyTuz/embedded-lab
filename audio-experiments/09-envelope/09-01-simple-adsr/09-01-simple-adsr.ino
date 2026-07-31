
#include <Arduino.h>
#include <SPI.h>
#include "sine.h"

#define LUT     fullSine

#define SPI_PORT    PORTB
#define NSS_PIN     53  // PB0
#define SCK_PIN     52  // PB1
#define MOSI_PIN    51  // PB2
#define MISO_PIN    50  // PB3

#define F_SAMPLE    16000   // sample rate: 16 kHz
#define F_OUT       880     // output audio frequency: 440Hz

// DDS variables
volatile uint16_t phase_accumulator = 0;
uint16_t phase_increment = (uint32_t) F_OUT * 65536UL / F_SAMPLE;
volatile uint8_t phase_index;
volatile uint16_t value; // really this needs to be 12 bits

volatile uint8_t do_output = 0;

typedef enum {
    ENV_IDLE,
    ENV_ATTACK,
    ENV_DECAY,
    ENV_RELEASE
} EnvelopeState;
/* This is a bit tricky. Given our sample/interrupt rate and the desired length of each of the envelope's
parts, I am trying to determine how much the envelope should change on each sample/interrupt. I'll call this
the envelope "delta" for each of the stages.

For example, if we want our attack to take 50 ms and we are using a 16 kHz sample rate, then the attack should
take 16 kHz * 50 ms = 800 samples. That means that over 800 samples, the envelope's amplitude needs to go from
0.0 to 1.0. Hence the attack stage needs a "delta" of 1.0 / 800 ~= 0.00125 amplitude units per sample.

Ideally on every sample/interrupt, we would increment the envelope amplitude by this "delta", then multiply
that (fraction) by the oscillator sample to get the enveloped value. However, we don't want to use floating
point values because those are very slow!

Instead of storing our envelope values as decimal values in [0,1], we can scale them by 2^16 = 65,536
and store that value in a 16-bit unsigned int. This allows us to get better precision with the steps.

That is, for our envelope values,
- the real value is x in [0,1]
- we store it as 65636 * x in a uint16
- when we want to apply it, we carefully do the multiplication and then divide by 65536.

For example, for the attack stage, we still need 800 samples, but now we want to go from 0 to 65,536 over those
800 samples; so the step size is 65536/800 = 81.92 ~= 82. That is, on every sample/interrupt, we will
increment envelope by 82 units.

Another example: for the decay stage, we want it to take 150 ms, so that's 16 kHz * 150 ms = 2400 steps.
We want the envelope amplitude to go from 1.0 to 0.7, so the step size is 0.3*65536/2400 = 8.192... ~= 9 units.
*/
#define ENV_MAX         65535
#define SUSTAIN_LEVEL   45875       // 70%
volatile uint32_t envelope = 0;     // Really only a 16-bit counter, but keeping 32 bits for arithmetic headroom

uint16_t env_attack_step = 82;   // 50 ms
uint16_t env_decay_step = 8;     // 150 ms to go from 1 to 0.70
uint16_t env_release_step = 15;  // 200 ms to go from 0.70 to 0

volatile EnvelopeState envState = ENV_IDLE;

/*
 * Update the value of the envelope.
 */
void update_envelope_value() {
    if (envState == ENV_IDLE) {
        return;
    } else if (envState == ENV_ATTACK) {
        envelope += env_attack_step;
        if (envelope >= ENV_MAX){
            envelope = ENV_MAX;
            envState = ENV_DECAY;
        }
    } else if (envState == ENV_DECAY) {
        envelope -= env_decay_step;
        if (envelope <= SUSTAIN_LEVEL){
            envelope = SUSTAIN_LEVEL;
            envState = ENV_RELEASE; // no sustain yet
        }
    } else if (envState == ENV_RELEASE) {
        if (envelope <= env_release_step) { 
            envelope = 0;
            envState = ENV_IDLE;
        } else {
            envelope -= env_release_step;
        }
    }
}


void configure_timer() {
    TCCR1A = 0;
    TCCR1B = 0;

    // Set WGM to CTC
    TCCR1B |= (1 << WGM12);
    // Keep output mode normal (not using hardware output pin)

    // Set prescaler=1
    TCCR1B |= (1 << CS10);

    // Set overflow compare register
    // OCR1A = 800; // F_CPU / F_SAMPLE
    OCR1A = (F_CPU / F_SAMPLE) - 1;
    // OCR1A = 799;

    // Enable interrupts on this timer
    TIMSK1 |= (1 << OCIE1A);

    sei();
}


/*
 * Take a sample from the waveform's lookup table.
 */
void sample_table() {
    phase_accumulator += phase_increment;
    // Shift to get the index (top 8 bits)
    phase_index = (phase_accumulator >> 8);
    // Index into the table
    uint16_t sample = 128 + LUT[phase_index];
    value = ((uint32_t)sample * 4095) / 255;
    
    // Get the envelope amplitude
    update_envelope_value();

    value = ((uint32_t)value * envelope) / ENV_MAX;
}

/*
 * Write the output 12-bit value to the DAC (via SPI).
 */
void output_to_dac() {
    uint16_t writeCmd = get_write_command(value);
    
    // digitalWrite(NSS_PIN, LOW);
    PORTB &= ~(1 << 0);
    SPI.transfer16(writeCmd);
    // digitalWrite(NSS_PIN, HIGH);
    PORTB |= (1 << 0);


}

/*
 * The ISR samples a signal value and outputs it to the DAC.
 */
ISR(TIMER1_COMPA_vect) {
    PINB |= (1 << PB7);   // toggle Arduino pin 13
    // Take a sample
    sample_table();
    // Output the current value
    output_to_dac();
}

void setup(){
    pinMode(3, OUTPUT);    // for tracing/debugging the interrupt rate
    pinMode(8, INPUT_PULLUP); // button

    Serial.begin(115200);

    // Serial.println(phase_increment);
    // Serial.println(OCR1A);
    // Serial.println(sizeof(fullSawtooth));
    // Serial.println(F_CPU, DEC);
    // Serial.println((uint32_t)F_CPU);
    Serial.println(phase_increment);

    // Start the SPI
    SPI.begin();

    // Set modes of the SPI pins
    pinMode(NSS_PIN, OUTPUT);
    pinMode(SCK_PIN, OUTPUT);
    pinMode(MOSI_PIN, OUTPUT);
    pinMode(MISO_PIN, INPUT);
    
    // Configure SPI to work with the MCP4921
    SPCR &= ~(1 << 5);  // DORD = MSB
    SPCR |= (1 << 4);   // MSTR = 1 (master)
    SPCR &= ~(1 << 3);  // CPOL = 0
    SPCR &= ~(1 << 2);  // CPHA = 0
    // SPCR |= (1 << 1);   // clock = fosc / 64
    // SPCR |= (0 << 1);
    SPCR &= ~(1 << SPR1);
    SPCR &= ~(1 << SPR0);
    SPSR |= (1 << SPI2X);

    /* Configure the timer */
    configure_timer();
    phase_accumulator = 0;
}

uint16_t get_write_command(uint16_t value){
    uint16_t writeCmd = value;

    // unset bit 15
    writeCmd &= ~(1 << 15);
    // buffer control bit
    writeCmd &= ~(1 << 14);
    // output gain selection
    writeCmd |= (1 << 13);  // x1
    // output shutdown control
    writeCmd |= (1 << 12);  // Vout active

    return writeCmd;
}

void loop(){
    if (digitalRead(8) == LOW && envState == ENV_IDLE) {
        // digitalWrite(3, !digitalRead(3));
        // delay(200);
        envState = ENV_ATTACK;
    }
}