/* Implementing a basic 5-step sequencer. */
#include <stdint.h>
#include <Arduino.h>


#define OUTPUT_PIN 8

unsigned long STEP_LENGTH_MS = 500;

// step values (0-256)
uint8_t step_vals[5] = {50, 100, 150, 200, 250};
unsigned long time_last_step_ms;

uint8_t current_step_idx;
bool step_event;


void setup() {
    pinMode(OUTPUT_PIN, OUTPUT);
    current_step_idx = 0;
    step_event = false;
    time_last_step_ms = millis();

    Serial.begin(115200);
}

void loop() {
    unsigned long now = millis();

    // detect if a step change has occurred
    if (now - time_last_step_ms > STEP_LENGTH_MS) {
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
}