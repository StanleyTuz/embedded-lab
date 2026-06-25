#define LED_PIN 13
#define BLINK_HALF_PERIOD 250

enum LedState {
  ON, OFF
};

unsigned long now;
unsigned long time_last_blinked_ms;
LedState state;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  state = OFF;
  time_last_blinked_ms = millis();
}

void change_state() {
  switch (state) {
    case ON:
      state = OFF;
      break;
    case OFF:
      state = ON;
      break;
  }
}

void update_led(){
  switch (state) {
    case ON:
      digitalWrite(LED_PIN, HIGH);
      break;
    case OFF:
      digitalWrite(LED_PIN, LOW);
      break;
  }
}

void loop() {
  now = millis();
  if (now - time_last_blinked_ms > BLINK_HALF_PERIOD) {
    time_last_blinked_ms = now;
    change_state();

    update_led();
  }
}


/* Now I will implement this same thing but with a bit more "code" around it, for practice. */
enum class BlinkState {
  ON, OFF
};

class BlinkController {
  private:
    BlinkState state;
    unsigned long last_blink_ms;
    unsigned long blink_half_period_ms;

  public:
    BlinkController(unsigned long blink_half_period_ms)
      : state(BlinkState::OFF), last_blink_ms(0), blink_half_period_ms(blink_half_period_ms) {}

    void update(unsigned long now) { // update internal state
      if (now - last_blink_ms > blink_half_period_ms) {
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
    }

    bool is_on() const {
      return (state == BlinkState::ON);
    }
};

class LedDriver {
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

unsigned long now;
BlinkController controller(100);
// BlinkController controller2(200);
LedDriver led(13);
// LedDriver led2(28);

void setup() {
  led.begin();
  // led2.begin();
}

void loop() {
  // Read "inputs"
  now = millis();

  // Update state
  controller.update(now);
  // controller2.update(now);
  
  // Write outputs
  led.set(controller.is_on());
  // led2.set(controller2.is_on());
}