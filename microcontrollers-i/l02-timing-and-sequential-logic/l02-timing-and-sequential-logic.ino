/* Lab 2A: Blink LED with delay()
  This implementation blinks an LED by using the Arduino's delay() function.
  This is "blocking" in the sense that the main execution path of our code
  is blocked while the delay waits. Interrupts can technically still interrupt
  delay(); Apparently "blocking" typically refers to "blocking the foreground code",
  not halting the entire microcontroller. So this is blocking because delay()
  stops loop() from progressing/iterating.
*/
// constexpr uint8_t LED_PIN = 12;
// constexpr uint16_t DELAY_TIME_MS = 500;

// void setup() {
//   // Configure the pin connected to an external LED
//   pinMode(LED_PIN, OUTPUT);
// }

// void loop() {
//   digitalWrite(LED_PIN, HIGH);
//   delay(DELAY_TIME_MS);
//   digitalWrite(LED_PIN, LOW);
//   delay(DELAY_TIME_MS);
// }


/* Lab 2B: Blink LED with millis().
  Instead of using delay() to block the loop execution, we continually poll the clock with millis()
  to determine when a timing threshold has been crossed. When it has, we flip the LED output.
  This is better, since now we can theoretically do other things within the loop().
*/
// constexpr uint8_t LED_PIN = 12;
// constexpr uint16_t DELAY_TIME_MS = 500;
// uint32_t time_last_switched_ms = 0;

// void setup() {
//   // Configure the pin connected to an external LED
//   pinMode(LED_PIN, OUTPUT);
//   // Record startup time
//   time_last_switched_ms = millis();
// }

// void loop() {

//   // Check if it's time to switch
//   if (millis() - time_last_switched_ms >= DELAY_TIME_MS) {
//     // Flip the state of the pin
//     // Note: this digitalRead is us effectively treating the hardware register as the source of truth.
//     // It may/may not be preferable to store LED state as a software variable, and then write it to
//     // hardware instead of relying on reading the state of the pin. This makes the code more extensible:
//     // if we want to introduce more states later on, we won't be able to determine the state simply by
//     // reading the pin value!
//     digitalWrite(LED_PIN, !digitalRead(LED_PIN));

//     time_last_switched_ms = millis();
//   }
// }

/* Trying this again, but I'll track the LED state in a variable instead of reading it from the hardware.
*/
// constexpr uint8_t LED_PIN = 12;
// constexpr uint16_t DELAY_TIME_MS = 500;
// uint32_t time_last_switched_ms = 0;
// bool led_on = false;

// void setup() {
//   // Configure the pin connected to an external LED
//   pinMode(LED_PIN, OUTPUT);
//   // Record startup time
//   time_last_switched_ms = millis();
// }

// void loop() {

//   // Check if it's time to switch
//   if (millis() - time_last_switched_ms >= DELAY_TIME_MS) {
//     // Flip the state of the pin
//     led_on = !led_on;
//     digitalWrite(LED_PIN, led_on);

//     time_last_switched_ms = millis();
//   }
// }

/* One more attempt, but explicitly using enums. Yes, I know that this is overkill.
*/
// constexpr uint8_t LED_PIN = 12;
// constexpr uint16_t DELAY_TIME_MS = 500;

// State is "all the information required to predict future behavior of the system"
// enum class LedState {
//   OFF, ON
// };

// LedState led_state; // single source of truth for the state; hardware reflects this

// uint32_t time_last_switched_ms = 0; // (technically I think state should include this too)


// void setup() {
//   // Configure the pin connected to an external LED
//   pinMode(LED_PIN, OUTPUT);
//   // Record startup time
//   time_last_switched_ms = millis();
//   led_state = LedState::OFF;
// }

// void loop() {
//   /* On every iteration, check if an event has occurred.
//   If so, update the state variables and then output to the hardware.
//   Trying to keep the "application state" and the "hardware state" separate.
//   */

//   // Detect if an "event" has occurred
//   if (millis() - time_last_switched_ms >= DELAY_TIME_MS) {
//     // Update state (led_state and time_last_switched_ms globals)
//     update_state();

//     // Update the hardware
//     update_outputs();
//   }
// }
// void update_outputs() {
//   digitalWrite(LED_PIN, led_state == LedState::ON ? HIGH : LOW);
// }

// void update_state() {
//   // Flip the state of the pin
//   switch (led_state) {
//     case LedState::OFF:
//       led_state = LedState::ON;
//       break;
//     case LedState::ON:
//       led_state = LedState::OFF;
//       break;
//   }
//   time_last_switched_ms = millis();
// }

/* One very final attempt, where I try to encapsulate all of my program's "internal state"
into a single class. I also tried to completely separate the hardware from the software by
parametrizing the update_state() function with the current time (in ms), instead of using
millis() inside it.
*/
// constexpr uint8_t LED_PIN = 12;
// constexpr uint16_t DELAY_TIME_MS = 500;

// // renaming this enum class to keep it disambiguated from the AppState below
// enum class LedMode {
//   OFF, ON
// };

// struct AppState {
//   LedMode mode;
//   uint32_t time_last_switched_ms;
// };

// uint32_t time_now_ms;
// AppState state;

// void setup() {
//   // Configure the pin connected to an external LED
//   pinMode(LED_PIN, OUTPUT);
//   time_now_ms = millis();
//   // Initialize the state variables
//   state = { LedMode::OFF, time_now_ms };
  
// }

// void loop() {
//   // Detect if an "event" has occurred
//   time_now_ms = millis();
//   if (time_now_ms - state.time_last_switched_ms >= DELAY_TIME_MS) {
//     // Update state (led_state and time_last_switched_ms globals)
//     update_state(time_now_ms);

//     // Update the hardware
//     update_outputs();
//   }
// }

// void update_outputs() {
//   digitalWrite(LED_PIN, state.mode == LedMode::ON ? HIGH : LOW);
// }

// void update_state(uint32_t t_now_ms) {
//   /* Assuming an event occurred, update the state variables (state transition).
//   I'm taking the current time (ms) as an input so as to keep internal state separated
//   from hardware. This is slightly more of a "pure function" now.
//   */

//   // Flip the state of the pin
//   state.mode = (state.mode == LedMode::ON ? LedMode::OFF : LedMode::ON); // collapsed for brevity
//   state.time_last_switched_ms = t_now_ms;
// }


// /* Lab 2C: Two LEDs blinking independently.

//   I can introduce a second LED by just duplicating the members in my AppState struct, and handling
//   them separately. This is effectively extending the "state vector" so that there is one big state
//   (4-)vector that maintains the state of both independent LEDs.
  
//   I can already see that this might be problematic, since it implies that if we ever want to add
//   more LEDs, etc., we will need to keep modifying the state struct. Even worse, it requires a lot
//   of duplicated logic. After this one, I will probably try it with a separate state struct for each LED.
// */
// constexpr uint8_t LED_PIN_A = 12;
// constexpr uint8_t LED_PIN_B = 13;
// constexpr uint16_t DELAY_TIME_MS_A = 500;
// constexpr uint16_t DELAY_TIME_MS_B = 250;

// enum class LedMode {
//   OFF, ON
// };

// // bundle both LEDs' states into one struct
// struct AppState {
//   LedMode mode_A;
//   uint32_t time_last_switched_ms_A;
//   LedMode mode_B;
//   uint32_t time_last_switched_ms_B;
// };

// uint32_t time_now_ms;
// AppState state;

// void setup() {
//   // Configure the pin connected to an external LED
//   pinMode(LED_PIN_A, OUTPUT);
//   pinMode(LED_PIN_B, OUTPUT);
//   time_now_ms = millis();
//   // Initialize the state variables
//   state = { LedMode::OFF, time_now_ms, LedMode::OFF, time_now_ms };
// }

// void loop() {
//   // Detect if an "event" has occurred
//   time_now_ms = millis();

//   if (time_now_ms - state.time_last_switched_ms_A >= DELAY_TIME_MS_A) {
//     update_state_A(time_now_ms);
//     update_outputs_A();
//   }
//   if (time_now_ms - state.time_last_switched_ms_B >= DELAY_TIME_MS_B) {
//     update_state_B(time_now_ms);
//     update_outputs_B();
//   }
// }


// // The duplication of these functions is annoying. I'm sure that if I thought that this
// // approach was best, I could parametrize this better.
// void update_outputs_A() {
//   digitalWrite(LED_PIN_A, state.mode_A == LedMode::ON ? HIGH : LOW);
// }
// void update_outputs_B() {
//   digitalWrite(LED_PIN_B, state.mode_B == LedMode::ON ? HIGH : LOW);
// }
// void update_state_A(uint32_t t_now_ms) {
//   /* Update the state of LED A. */
//   state.mode_A = (state.mode_A == LedMode::ON ? LedMode::OFF : LedMode::ON);
//   state.time_last_switched_ms_A = t_now_ms;
// }
// void update_state_B(uint32_t t_now_ms) {
//   /* Update the state of LED B. */
//   state.mode_B = (state.mode_B == LedMode::ON ? LedMode::OFF : LedMode::ON);
//   state.time_last_switched_ms_B = t_now_ms;
// }

/* Try this again, but now I will create an LedState struct to represent a single LED, and then
use AppState to contain the LedState for both LEDs. This allows me to reduce a lot of the duplicate
code; in particular, the update_state() function can now operate on a single LED at a time.

However, there is still duplication in how we're checking for timing, and how we update the outputs (pins).
I am thinking I could improve this by somehow bundling up the "configuration" of each pin --- LED_PIN_* and
DELAY_TIME_MS_* --- with the pins, and passing these around as well. But I don't want to spend much more
time on this.
*/
constexpr uint8_t LED_PIN_A = 12;
constexpr uint8_t LED_PIN_B = 13;
constexpr uint16_t DELAY_TIME_MS_A = 500;
constexpr uint16_t DELAY_TIME_MS_B = 250;

enum class LedMode {
  OFF, ON
};

struct LedState {
  LedMode mode;
  uint32_t time_last_switched_ms;
};

struct AppState {
  LedState led_A;
  LedState led_B;
};

uint32_t time_now_ms;
AppState state;

LedState update_led_state(LedState, uint32_t);
void update_outputs();

void setup() {
  // Configure the pin connected to an external LED
  pinMode(LED_PIN_A, OUTPUT);
  pinMode(LED_PIN_B, OUTPUT);
  time_now_ms = millis();
  // Initialize the state variables
  state = { {LedMode::OFF, time_now_ms}, {LedMode::OFF, time_now_ms} };
}

void loop() {
  // Detect if an "event" has occurred
  time_now_ms = millis();

  if (time_now_ms - state.led_A.time_last_switched_ms >= DELAY_TIME_MS_A) {
    state.led_A = update_led_state(state.led_A, time_now_ms);
  }
  if (time_now_ms - state.led_B.time_last_switched_ms >= DELAY_TIME_MS_B) {
    state.led_B = update_led_state(state.led_B, time_now_ms);
  }
  
  update_outputs();
}

LedState update_led_state(LedState led_state, uint32_t t_now_ms) {
  led_state.mode = (led_state.mode == LedMode::ON ? LedMode::OFF : LedMode::ON);
  led_state.time_last_switched_ms = t_now_ms;
  return led_state;
}

void update_outputs() {
  digitalWrite(LED_PIN_A, state.led_A.mode == LedMode::ON ? HIGH : LOW);
  digitalWrite(LED_PIN_B, state.led_B.mode == LedMode::ON ? HIGH : LOW);
}