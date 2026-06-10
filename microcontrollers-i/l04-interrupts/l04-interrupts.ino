/* Lab 4A: Button read with polling.

  Polling: "actively sampling the status of an external device by a
  client program as a synchronous activity.
  
  I've done this several times already, but the idea is to read
  a button press (a pin's value) by checking its value on every
  iteration of the loop.
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

/* Lab 4B: Button read with interrupts.

  Instead of having the CPU (loop()) actively polling for the
  button press, we can register it as an interrupt.
  
  I've done this several times already, but the idea is to read
  a button press (a pin's value) by checking its value on every
  iteration of the loop.
*/

constexpr uint16_t BUTTON_PIN = 2; // moved this to an interrupt-capable pin
constexpr uint16_t LED_PIN = 13;
constexpr uint8_t DEBOUNCE_TIME_MS = 50;

// This is the actual state of the system
enum class LedState {
  ON, OFF
};

volatile bool button_pressed = false;
unsigned long time_last_pressed;
LedState led_state;

void setup() {
  // Configure the pins
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  
  // Set initial conditions
  led_state = LedState::OFF;
  button_pressed = false;
  time_last_pressed = millis();

  // Apply the ICs
  update_outputs();

  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), button_isr, FALLING);
}

// This callback is invoked when the button is pressed
// Mode (third argument) can be LOW, CHANGE, RISING, FALLING
void button_isr() {
  button_pressed = true;
}

void loop() {
  if (button_pressed) {
    // Note: an interrupt could technically interrupt this block!
    button_pressed = false; // reset the interrupt trigger
    
    // Debouncing
    unsigned long now = millis();
    if (now - time_last_pressed > DEBOUNCE_TIME_MS) {
      time_last_pressed = now;
      led_state = (led_state == LedState::OFF) ? LedState::ON : LedState::OFF; // toggle LED state
      update_outputs();
    }
    
  }
}

void update_outputs() {
  digitalWrite(LED_PIN, (led_state == LedState::ON) ? HIGH : LOW);
}

/* With polling, loop() detects the edge.
With interrupts, the ISR detects the edge and the loop() handles the event.
*/