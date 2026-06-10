/* Lab 3A: Button toggle.

  Use a button press to toggle an LED. Simple example of a FSM.

  The key is that the FSM class owns the state, and produces "intent".
  Later we could implement a hardware class that takes that "instruction"
  and manipulates the pins; this is better than letting the FSM do this
  directly.
*/

// constexpr uint16_t BUTTON_PIN = 12;
// constexpr uint16_t LED_PIN = 13;

// // This is the actual state of the system
// enum class LedState {
//   ON, OFF
// };

// // The button's "state" is not part of the actual state of the system!
// // It is just used for edge detection.
// enum class ButtonLevel {
//   ON, OFF
// };

// LedState led_state;
// ButtonLevel previous_button_level;
// ButtonLevel current_button_level;

// void setup() {
//   // Configure the pins
//   pinMode(BUTTON_PIN, INPUT_PULLUP);
//   pinMode(LED_PIN, OUTPUT);
  
//   // Set initial conditions
//   led_state = LedState::OFF;
//   previous_button_level = ButtonLevel::OFF;
//   current_button_level = ButtonLevel::OFF;

//   // Apply the ICs
//   update_outputs();
// }

// void loop() {
//   // Read the value of the button
//   current_button_level = (digitalRead(BUTTON_PIN) == LOW ? ButtonLevel::ON : ButtonLevel::OFF);

//   // Check for a "rising edge"
//   if ((current_button_level == ButtonLevel::ON)&&(previous_button_level == ButtonLevel::OFF)) {
//     // toggle the state
//     led_state = (led_state == LedState::OFF ? LedState::ON : LedState::OFF);
//     update_outputs();
//   }
//   previous_button_level = current_button_level;
  
// }

// void update_outputs() {
//   digitalWrite(LED_PIN,
//     (led_state == LedState::ON) ? HIGH : LOW
//   );
// }




/* Lab 3C: Three-mode LED controller, where button cycles modes.

  This FSM has three state: OFF, ON, and BLINKING. Note that while
  BLINKING the LED requires tracking time and toggling the LED, this
  is an "internal implementation state" of the blinking mode.
*/
constexpr uint8_t LED_PIN = 13;
constexpr uint8_t BUTTON_PIN = 12;
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
  pinMode(12, INPUT_PULLUP);
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


