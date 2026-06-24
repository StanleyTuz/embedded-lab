// #define LED_PIN 13
// #define BLINK_HALF_PERIOD 250

// enum LedState {
//   ON, OFF
// };

// unsigned long now;
// unsigned long time_last_blinked_ms;
// LedState state;

// void setup() {
//   pinMode(LED_PIN, OUTPUT);
//   digitalWrite(LED_PIN, LOW);
//   state = OFF;
//   time_last_blinked_ms = millis();
// }

// void change_state() {
//   switch (state) {
//     case ON:
//       state = OFF;
//       break;
//     case OFF:
//       state = ON;
//       break;
//   }
// }

// void update_led(){
//   switch (state) {
//     case ON:
//       digitalWrite(LED_PIN, HIGH);
//       break;
//     case OFF:
//       digitalWrite(LED_PIN, LOW);
//       break;
//   }
// }

// void loop() {
//   now = millis();
//   if (now - time_last_blinked_ms > BLINK_HALF_PERIOD) {
//     time_last_blinked_ms = now;
//     change_state();

//     update_led();
//   }
// }


// /* Now I will implement this same thing but with a bit more "code" around it, for practice. */
// enum class BlinkState {
//   ON, OFF
// };

// class BlinkController {
//   private:
//     BlinkState state;
//     unsigned long last_blink_ms;
//     unsigned long blink_half_period_ms;

//   public:
//     BlinkController(unsigned long blink_half_period_ms)
//       : state(BlinkState::OFF), last_blink_ms(0), blink_half_period_ms(blink_half_period_ms) {}

//     void update(unsigned long now) { // update internal state
//       if (now - last_blink_ms > blink_half_period_ms) {
//         last_blink_ms = now;
//         switch (state) {
//           case BlinkState::ON:
//             state = BlinkState::OFF;
//             break;
//           case BlinkState::OFF:
//             state = BlinkState::ON;
//             break;
//         }
//       }
//     }

//     bool is_on() const {
//       return (state == BlinkState::ON);
//     }
// };

// class LedDriver {
//   private:
//     uint8_t pin;
  
//   public:
//     LedDriver(uint8_t pin) : pin(pin) {}

//     void set(bool on) {
//       digitalWrite(pin, on ? HIGH : LOW);
//     }

//     void begin() {
//       pinMode(pin, OUTPUT);
//       set(false);
//     }
// };

// unsigned long now;
// BlinkController controller(100);
// // BlinkController controller2(200);
// LedDriver led(13);
// // LedDriver led2(28);

// void setup() {
//   led.begin();
//   // led2.begin();
// }

// void loop() {
//   // Read "inputs"
//   now = millis();

//   // Update state
//   controller.update(now);
//   // controller2.update(now);
  
//   // Write outputs
//   led.set(controller.is_on());
//   // led2.set(controller2.is_on());
// }


/* Lab 3C: Three-mode LED controller, where button cycles modes.

  This FSM has three state: OFF, ON, and BLINKING. Note that while
  BLINKING the LED requires tracking time and toggling the LED, this
  is an "internal implementation state" of the blinking mode.
*/
constexpr uint8_t LED_PIN = 13;
constexpr uint8_t BUTTON_PIN = 11;
constexpr uint8_t BLINK_DELAY_MS = 100;
constexpr uint8_t DEBOUNCE_MS = 50;

// This is the high-level state of the system.
enum class Mode { OFF, ON, BLINKING };

enum class ButtonLevel {
  ON, OFF
};

class LedFSM {
  public:
    LedFSM() : mode(Mode::OFF), blink_led_state(false), last_toggle_time(0) {}

    void onEvent() {
      /* When loop() has detected an event --- a button press --- we transition
      between states. This is simple here: we just cycle through them. */
      switch (mode) {
        case Mode::OFF:
          this->mode = Mode::ON;
          break;
        case Mode::ON:
          this->mode = Mode::BLINKING;
          break;
        case Mode::BLINKING:
          this->mode = Mode::OFF;
          break; 
      }
    };

    void update() {
      /* On every iteration of the loop(), this function is called and implements
      state-specific behavior. If the mode is ON, we keep the LED on. If the mode
      is OFF, we keep the LED off. If the mode is BLINKING, then we perform some
      standard blinking logic.

      Note: ideally I would remove the hardware output from this method and this
      entire class, so that the FSM just contains logic. I would have update()
      simply emit an "instruction" that another object --- in loop() --- would
      use to do actuation of the hardware. That will just make this example messy,
      so I'll omit it for now. I would also make update() take millis() as an input.
      */
      uint16_t now = millis();
      switch (mode) {
        case Mode::OFF:
          digitalWrite(LED_PIN, LOW);
          break;
        
        case Mode::ON:
          digitalWrite(LED_PIN, HIGH);
          break;
        
        case Mode::BLINKING:
          do_blink(now);
          break;
      }
    }

  void do_blink(uint16_t now) {
    if (now - last_toggle_time > BLINK_DELAY_MS) {
      last_toggle_time = now;
      blink_led_state = !blink_led_state;
    }
    digitalWrite(LED_PIN,
      (blink_led_state) ? HIGH : LOW
    );
  }

  private:
    Mode mode;

    // internal "sub-state" for the blinking mode
    bool blink_led_state;
    unsigned long last_toggle_time;
};

ButtonLevel previous_button_level;
ButtonLevel current_button_level;
unsigned long last_press_time_ms; // used to de-bounce button press 
LedFSM led_fsm = LedFSM();

void setup() {
  pinMode(13, OUTPUT);
  pinMode(11, INPUT_PULLUP);
  last_press_time_ms = millis();
}

void loop() {
  // Read the value of the button
  current_button_level = (digitalRead(BUTTON_PIN) == LOW ? ButtonLevel::ON : ButtonLevel::OFF);

  // Check for a "rising edge", which would change the mode
  if ((current_button_level == ButtonLevel::ON)&&(previous_button_level == ButtonLevel::OFF)) {
    // Debouncing: check if we have had a button press very recently. If so, ignore this one.
    unsigned long now = millis();
    if (now - last_press_time_ms > DEBOUNCE_MS) {
      last_press_time_ms = now;
      // send the button press event to the FSM
      led_fsm.onEvent();
    }
  }
  previous_button_level = current_button_level;
  
  // Update the hardware
  led_fsm.update();
}


