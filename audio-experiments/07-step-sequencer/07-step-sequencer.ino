/* Implementing a basic 5-step sequencer. */
#include <stdint.h>
#include <Arduino.h>


#define OUTPUT_PIN 8
#define RATE_POT_PIN A0

#define DELAY_READ_RATE_KNOB 50

unsigned long STEP_LENGTH_RANGE_MS[2] = {10, 1000};
unsigned long step_length_ms;
unsigned long time_last_rate_knob_read_ms;

// step values (0-256)
uint8_t step_vals[5] = {50, 100, 150, 200, 250};
unsigned long time_last_step_ms;

uint8_t current_step_idx;
bool step_event;

int read_rate_knob() {
    // Read raw value
    int raw = analogRead(RATE_POT_PIN); // 0 - 1023

    // Scale to the range
    return STEP_LENGTH_RANGE_MS[0] + (int)(raw * (STEP_LENGTH_RANGE_MS[1] - STEP_LENGTH_RANGE_MS[0]) / 1023.0);
}

void setup() {
    pinMode(OUTPUT_PIN, OUTPUT);
    pinMode(RATE_POT_PIN, INPUT);

    current_step_idx = 0;
    step_event = false;
    time_last_step_ms = millis();
    time_last_rate_knob_read_ms = millis();
    step_length_ms = read_rate_knob();

    Serial.begin(115200);
}

void loop() {
    unsigned long now = millis();

    // detect if a step change has occurred
    if (now - time_last_step_ms > step_length_ms) {
        step_event = true;
        time_last_step_ms = now;
    }

    // Change step
    if (step_event) {
        step_event = false; // reset
        current_step_idx = (current_step_idx + 1) % 5;
        
        // Update the output
        uint8_t output_val = step_vals[current_step_idx];
        analogWrite(OUTPUT_PIN, output_val);
        Serial.print("Output val has changed to ");
        Serial.println(output_val);
    }

    // If it's time to sample the rate pot, do so
    if (now - time_last_rate_knob_read_ms > DELAY_READ_RATE_KNOB) {
        int new_rate = read_rate_knob();
        step_length_ms = new_rate;
    }
}