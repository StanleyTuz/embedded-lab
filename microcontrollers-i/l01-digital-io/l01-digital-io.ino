// /* Lab 1A: Blink LED ---
//   Note that this implementation doesn't use "state" at all. In fact, it's blocking with the delay() call.
//   A better implementation would poll on every iteration of the loop to see if it's time to toggle the LED.
//     This would prevent blocking.
//   An even better implementation would be to configure a hardware timer to interrupt every 500 ms. When this
//     fires, we toggle the state.
// */
// constexpr uint8_t LED_PIN = 12; // fixed
// // or use 13 to see the internal LED blink too
// constexpr uint16_t ON_TIME_MS = 500;
// constexpr uint16_t OFF_TIME_MS = 500;

// void setup() {
//   // Configure the pin connected to an external LED
//   pinMode(LED_PIN, OUTPUT);
// }

// void loop() {
//   digitalWrite(LED_PIN, HIGH);
//   delay(ON_TIME_MS);
//   digitalWrite(LED_PIN, LOW);
//   delay(OFF_TIME_MS);
// }


/* Lab 1B: Control LED with button.

  My first thought was to wire a pin through a push-button switch, a resistor, and an LED in series, to ground.
  Then we just set the pin to HIGH. Thus when the button is pressed, the circuit is closed, and the LED will light.
  But this is not very interesting at all, since it's simply using the microcontroller like a battery.

  Instead, I will use one pin as a (digital) input, to determine if the button is pressed or not --- detecting hardware
  state. Another pin will deliver power to the LED according to the reading of the button pin. Since the push-button will
  be wired between the "button pin" and ground, the button pin will be pulled LOW (to ground) when the button is pressed;
  thus I will set a pull-up resistor to make this pin be pulled HIGH when the button is unpressed.

  Note: this is a bit wasteful because we are writing to the output pin on every iteration of the loop.
  Note also that this is *implicitly* state-based, because I am taking action based on the state of the LED/button.
  Every iteration, I check the current state of the button and then force the LED into the corresponding state.
  After this, I'll implement the same thing in an edge-sensitive way, so that I only do writes when the state changes.

  Much later, I can probably implement this in an event-driven way instead of a fully state-based way: a button press or release triggers
  an interrupt, and the state (output pin voltage) is updated accordingly.
*/

// constexpr uint8_t PIN_BUTTON = 9;
// constexpr uint8_t PIN_LED = 12;

// void setup() {
//   pinMode(PIN_BUTTON, INPUT_PULLUP);
//   pinMode(PIN_LED, OUTPUT);
// }

// void loop() {
//   // If the button is being pressed, turn on the LED
//   if (digitalRead(PIN_BUTTON) == LOW) {
//     digitalWrite(PIN_LED, HIGH);
//   }
//   else {
//     digitalWrite(PIN_LED, LOW);
//   }
// }

/* Here I implement the same thing but I try to do "edge detection" using states. This is sort of sitting
between fully state-based and fully event-driven approaches, since it explicitly detects events but then
uses states to */

// constexpr uint8_t PIN_BUTTON = 9;
// constexpr uint8_t PIN_LED = 12;

// bool previous_button_state = LOW;
// bool current_button_state = LOW;

// void setup() {
//   pinMode(PIN_BUTTON, INPUT_PULLUP);
//   pinMode(PIN_LED, OUTPUT);
// }

// void loop() {
//   // Read the current state of the pin
//   current_button_state = digitalRead(PIN_BUTTON);
//   // If it has just changed (an edge), then update the LED pin accordingly
//   if (current_button_state != previous_button_state) { // detect an event
//     // use state to take an action
//     digitalWrite(PIN_LED, current_button_state == LOW ? HIGH : LOW); // enforces LED = NOT(Button)

//     previous_button_state = current_button_state; // prep for next iteration
//   }
// }





/* Lab 1C: Observe floating inputs.

  This is the same code as above, but I am removing the pullup resistor from the button input pin.
  This means that when the button is not pressed, it is no longer being (weakly) pulled HIGH. In fact,
  when I run this I see that the LED is always on, but not really at full brightness, and it appears to
  flicker. This is because it isn't being held HIGH. When I press the button, the LED lights up fully,
  still, which makes sense.
*/

constexpr uint8_t PIN_BUTTON = 9;
constexpr uint8_t PIN_LED = 12;

void setup() {
  pinMode(PIN_BUTTON, INPUT); // not INPUT_PULLUP
  pinMode(PIN_LED, OUTPUT);
}

void loop() {
  // If the button is being pressed, turn on the LED
  if (digitalRead(PIN_BUTTON) == LOW) {
    digitalWrite(PIN_LED, HIGH);
  }
  else {
    digitalWrite(PIN_LED, LOW);
  }
}





