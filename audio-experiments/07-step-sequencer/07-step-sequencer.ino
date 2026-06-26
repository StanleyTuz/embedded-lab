/* Implementing a basic 5-step sequencer. */
#include <stdint.h>
#include <Arduino.h>


#define OUTPUT_PIN 8
#define RATE_POT_PIN A0
#define MODE_SELECT_PIN 50

#define DELAY_READ_RATE_KNOB 50

enum class StepModeLevel {
    STEP3, STEP4, STEP5
};

class StepMode {
    private:
        StepModeLevel level = StepModeLevel::STEP4;
        uint8_t _max_steps = 4;

        
    public:
        StepMode(){}
        
        uint8_t _calc_max_steps(StepModeLevel level) {
            switch (level) {
                case StepModeLevel::STEP3:
                    return 3;
                    break;
                case StepModeLevel::STEP4:
                    return 4;
                    break;
                case StepModeLevel::STEP5:
                    return 5;
                    break;
            }
        }

        void set_level(StepModeLevel level) {
            this->level = level;
            this->_max_steps = _calc_max_steps(level);
        }

        void advance_level() {
            switch (level) {
                case StepModeLevel::STEP3:
                    set_level(StepModeLevel::STEP4);
                    break;
                case StepModeLevel::STEP4:
                    set_level(StepModeLevel::STEP5);
                    break;
                case StepModeLevel::STEP5:
                    set_level(StepModeLevel::STEP3);
                    break;
            }
        }

        uint8_t const max_steps() {
            return _max_steps;
        }
};

unsigned long STEP_LENGTH_RANGE_MS[2] = {10, 1000};
unsigned long step_length_ms;
unsigned long time_last_rate_knob_read_ms;

// step values (0-256)
uint8_t step_vals[5] = {50, 100, 150, 200, 250};
unsigned long time_last_step_ms;

StepMode step_mode;
uint8_t current_step_idx;
bool step_event;

bool mode_button_pressed;
bool mode_button_pressed_previously;

int read_rate_knob() {
    // Read raw value
    int raw = analogRead(RATE_POT_PIN); // 0 - 1023

    // Scale to the range
    return STEP_LENGTH_RANGE_MS[0] + (int)(raw * (STEP_LENGTH_RANGE_MS[1] - STEP_LENGTH_RANGE_MS[0]) / 1023.0);
}


void change_step() {
    current_step_idx = (current_step_idx + 1) % step_mode.max_steps();
}

void setup() {
    pinMode(OUTPUT_PIN, OUTPUT);
    pinMode(RATE_POT_PIN, INPUT);
    pinMode(MODE_SELECT_PIN, INPUT_PULLUP);

    step_mode = StepMode(); // defaults to 4 steps

    current_step_idx = 0;
    step_event = false;
    time_last_step_ms = millis();
    time_last_rate_knob_read_ms = millis();
    step_length_ms = read_rate_knob();

    mode_button_pressed = false;
    mode_button_pressed_previously = false;

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
        change_step(); // Update the current_step_idx
        
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

    // Check if the step mode button has been pressed
    mode_button_pressed_previously = mode_button_pressed;
    mode_button_pressed = (digitalRead(MODE_SELECT_PIN) == LOW);
    if (mode_button_pressed && !mode_button_pressed_previously) {
        step_mode.advance_level();
    }
}