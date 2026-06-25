
enum class LedMode { OFF, ON, BLINKING };

enum class ButtonLevel {
  ON, OFF
};

enum class BlinkState { OFF, ON };

class LedDriver {
  /* Interface with the actual hardware. Turns LEDs on and off. */
  private:
    uint8_t pin;

  public:
    LedDriver(uint8_t pin) : pin(pin) {}

    void set(bool on) {
      digitalWrite(pin, on ? HIGH : LOW);
    }

    void begin() {
      pinMode(pin, OUTPUT);
      set(false);
    }
};

class ButtonDriver {
  /* Interface with the button. Includes de-bouncing logic. */
  private:
    uint8_t pin;

    ButtonLevel stable_state = ButtonLevel::OFF;
    ButtonLevel last_raw_read = ButtonLevel::OFF;
    
    unsigned long last_change_ms = 0;  
    const unsigned long debounce_ms = 50;

    // Indicate if an event just occurred
    // Note: methods will reset these once "consumed"
    bool release_event = false;
    bool press_event = false;

  public:
    ButtonDriver(uint8_t pin) : pin(pin) {}

    void begin() {
      pinMode(pin, INPUT_PULLUP);
    }

    bool const isPressed() {
      return (stable_state == ButtonLevel::ON);
    }

    bool wasPressed() {
      // reset event flags  
      if (press_event) {
        press_event = false;
        return true;
      }
      return false;
    }

    bool wasReleased() {
      // reset event flags
      if (release_event) {
        release_event = false;
        return true;
      }
      return false; 
    }

    void update(unsigned long now) {
      // Read the button
      ButtonLevel raw = (digitalRead(pin) == LOW ? ButtonLevel::ON : ButtonLevel::OFF);

      // Detect raw changes
      if (raw != last_raw_read) {
        last_change_ms = now;
        last_raw_read = raw;
      }

      // If the read is stable long enough, update the state
      if ((now - last_change_ms) > debounce_ms && raw != stable_state) {
        stable_state = raw;
        
        // If we just changed to a pressed state
        if (stable_state == ButtonLevel::ON) {
          press_event = true;
        } else {
          release_event = true;
        }
      }
    }
};

class ModeController {
  /* State machine logic that encodes the state transitions.
  Only emits the current (desired) state of the LED.
  */
  private:
  
  public:
    LedMode mode;
    ModeController() : mode(LedMode::OFF) {}

    void advance() {
      /* When loop() has detected an event --- a button press --- we transition
      between states. This is simple here: we just cycle through them. */
      switch (mode) {
        case LedMode::OFF:
          mode = LedMode::ON;
          break;
        case LedMode::ON:
          mode = LedMode::BLINKING;
          break;
        case LedMode::BLINKING:
          mode = LedMode::OFF;
          break; 
      }
    }
};

class BlinkEngine {
  private:
    BlinkState state;
    unsigned long last_blink_ms;
    unsigned long blink_half_period_ms;
  
  public:
    BlinkEngine(unsigned long half_period_ms) : state(BlinkState::OFF), last_blink_ms(0), blink_half_period_ms(half_period_ms) {}

    bool update(unsigned long now) {
      if (now - last_blink_ms > blink_half_period_ms) {
        // change state
        last_blink_ms = now;
        switch (state) {
          case BlinkState::ON:
            state = BlinkState::OFF;
            break;
          case BlinkState::OFF:
            state = BlinkState::ON;
            break;
        }
      }

      return (state == BlinkState::ON ? true : false);
    }
};


ModeController mode_controller = ModeController();
ButtonDriver button = ButtonDriver(11);
LedDriver led = LedDriver(13);
BlinkEngine blinker = BlinkEngine(500);

void setup() {
  led.begin();
  button.begin();
}

void loop() {
  unsigned long now = millis();
  
  // Read the value of the button
  button.update(now);

  // If an event occurred, controller must change state
  if (button.wasPressed()) {
    mode_controller.advance();
  }
  
  // Update the hardware
  switch (mode_controller.mode) { 
    case LedMode::OFF:
      led.set(false);
      break;
    case LedMode::ON:
      led.set(true);
      break;
    case LedMode::BLINKING:
      led.set(blinker.update(now));
      break;
  }
}
