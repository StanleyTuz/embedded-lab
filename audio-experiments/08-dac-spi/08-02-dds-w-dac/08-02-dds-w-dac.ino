
#include <Arduino.h>
#include <SPI.h>
#include "sine.h"

#define LUT     fullSine

#define SPI_PORT    PORTB
#define NSS_PIN     53  // PB0
#define SCK_PIN     52  // PB1
#define MOSI_PIN    51  // PB2
#define MISO_PIN    50  // PB3

#define F_SAMPLE    16000   // sample rate: 20kHz
#define F_OUT       400     // output audio frequency: 440Hz

// DDS variables
volatile uint16_t phase_accumulator = 0;
uint16_t phase_increment = (uint32_t) F_OUT * 65536UL / F_SAMPLE;
volatile uint8_t phase_index;
volatile uint16_t value; // really this needs to be 12 bits

volatile uint8_t do_output = 0;

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
    // value = 4095;
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
    pinMode(13, OUTPUT);

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
}