/*

This should be _non-blocking_: the loop runs freely, but we do checks on each iteration.

However it is not very interrupt-safe or real-time deterministic.

FSM: state machine, time-based transitions, no blocking delays.
This is: a loop that periodically checks time and updates state.
(state, timers) -> next state

More advanced would be: a system reacting to events (time, interrupts, sensors) with strict rules about timing and shared data safety.
Let's add an interrupt.
(state, timers, external interrupt flag) -> next state

Good embedded design is about making state explicit.
State diagram, event table, timing rules.

Embedded programming is largely about managing time, events, and state explicitly, not "writing clever code".
*/


/*
Event semantics must be defined explicitly
- one shot event (consume once ever)
- level event (stay true until handled)
- latched event (stay true until explicitly cleared)
*/

#define PED_BUTTON_PIN 2  // can only use pins 2, 3, and some others

static constexpr unsigned long GREEN_TIME_MS = 5000;
static constexpr unsigned long YELLOW_TIME_MS = 2000;
static constexpr unsigned long BOTH_RED_TIME_MS = 1000;

struct Pins {
  int aRed, aYellow, aGreen;
  int bRed, bYellow, bGreen;
};

enum class State {
  A_GREEN,  // Light A is green
  A_YELLOW, // Light A is yellow
  BOTH_RED_TO_B, // Both lights are red, B is about to turn green
  B_GREEN,  // Light B is green
  B_YELLOW,  // Light B is yellow
  BOTH_RED_TO_A // Both lights are red, A is about to turn green
};

struct FSMState {
  State currentState;
  unsigned long startTime;
  bool pedestrianWaiting;
};

struct Command {
  /* Represent the outputs from the controller. These are "commands" in that
  they command the hardware to behave in a certain way. */
  bool aRed;
  bool aYellow;
  bool aGreen;
  bool bRed;
  bool bYellow;
  bool bGreen;

  // overloaded operator for easy checking of state changes
  bool operator==(const Command& other) const {
    // second const means that this method won't mutate this
    return 
      aRed == other.aRed &&
      aYellow == other.aYellow && 
      aGreen == other.aGreen && 
      bRed == other.bRed &&
      bYellow == other.bYellow && 
      bGreen == other.bGreen;
  }
};

// Interrupt 
volatile bool pedestrianRequested {false};
void onPedestrianButtonPress() {
  /* Callback AKA Interrupt Service Routine (ISR) AKA interrupt handler.
  
  This is a "callback" because it is a function that is passed as data to
  another function (attachInterrupt) which calls it later (callback pattern).
  */
  pedestrianRequested = true;
}


class TrafficLightHardware {
  /* Hardware-facing object that configures and communicates with the Arduino pins.*/
  private:
    Pins pins;  // store the pin assignments

  public:
    TrafficLightHardware(const Pins& pins)
      : pins(pins) {}
    
    void begin() {
      /* Initialize hardware by setting the pin output modes. */
      pinMode(pins.aRed, OUTPUT);
      pinMode(pins.aYellow, OUTPUT);
      pinMode(pins.aGreen, OUTPUT);
      pinMode(pins.bRed, OUTPUT);
      pinMode(pins.bYellow, OUTPUT);
      pinMode(pins.bGreen, OUTPUT);
      pinMode(PED_BUTTON_PIN, INPUT_PULLUP);
    }

    void applyCommand(const Command& out) {
      /* Given output */
      digitalWrite(pins.aRed, out.aRed);
      digitalWrite(pins.aYellow, out.aYellow);
      digitalWrite(pins.aGreen, out.aGreen);
      digitalWrite(pins.bRed, out.bRed);
      digitalWrite(pins.bYellow, out.bYellow);
      digitalWrite(pins.bGreen, out.bGreen);
    }

};


class TrafficLightController {
  /* This is the FSM; the owner of the notion of "what state the system is in". */
  private:
    FSMState state;

  public:
    /* Constructor: configure object state. Standard C++ setup. Safe to run before runtime starts.
    Things that are just data; internal variables; configuration; pure logic.
    "Build object in memory"
    */
    TrafficLightController()
      : state{
        State::A_GREEN,  // currentState
        0,        // startTime
        false     // pedestrianWaiting
      } {}

    /* begin(): init hardware/runtime resources; Arduino/MCU setup; requires MCU runtime to exist.
    Things that touch hardware, runtime, peripherals, interrupts, timers, serial, GPIO config.
    "Activate the hardware"
    */
    void begin() {
      // Set start time
      state.startTime = millis();
    }

    /* Now it's more of a pure function! */
    Command update(unsigned long now) {
      // Contains state-switching logic

      switch (state.currentState) {
        // If A is green and it's time to turn yellow
        case State::A_GREEN:
          if ((now - state.startTime > GREEN_TIME_MS)
            || state.pedestrianWaiting ) {
            // edge-triggered consumption
            state.pedestrianWaiting = false;
            state.currentState = State::A_YELLOW;
            state.startTime = now;
          }
          break;
        case State::A_YELLOW:
          // If A has been yellow long enough, A turns red
          // in preparation of B turning green
          if (now - state.startTime > YELLOW_TIME_MS) {
            state.currentState = State::BOTH_RED_TO_B;
            state.startTime = now;
          }
          break;
        case State::BOTH_RED_TO_B:
          // If both are red and B is ready to turn on
          if (now - state.startTime > BOTH_RED_TIME_MS) {
            state.currentState = State::B_GREEN;
            state.startTime = now;
          }
          break;
        case State::B_GREEN:
          if (now - state.startTime > GREEN_TIME_MS) {
            state.currentState = State::B_YELLOW;
            state.startTime = now;
          }
          break;
        case State::B_YELLOW:
          if (now - state.startTime > YELLOW_TIME_MS) {
            state.currentState = State::BOTH_RED_TO_A;
            state.startTime = now;
          }
          break;
        case State::BOTH_RED_TO_A:
          if (now - state.startTime > BOTH_RED_TIME_MS) {
            state.currentState = State::A_GREEN;
            state.startTime = now;
          }
          break;
      }
      return produceCommand();
    }

    Command produceCommand() {
      /* Produce output instructions to tell the hardware what to do. */
      bool aTurnGreen = (state.currentState == State::A_GREEN);
      bool aTurnYellow = (state.currentState == State::A_YELLOW);

      bool bTurnGreen = (state.currentState == State::B_GREEN);
      bool bTurnYellow = (state.currentState == State::B_YELLOW);

      // Brace constructor --- how does it know what object to construct?
      return {
        !(aTurnGreen || aTurnYellow),
        aTurnYellow,
        aTurnGreen,
        !(bTurnGreen || bTurnYellow),
        bTurnYellow,
        bTurnGreen
      };
    }

    // button press -> set flag True -> stay True until consumed (latched request model)
    // OTHER: button press is only meaningful at certain times, otherwise it's ignored (edge-gated event model)
    void requestPedestrianCrossing() {
      // When a pedestrian requests a crossing, the controller stores
      // the event internally

      // Note: This is making it a edge-(or state-)gated model instead of a pure latched model (persistent request model)
      // The input function depends on the current state now; "input semantics have become state-dependent"
      if (state.currentState == State::A_GREEN) {
        state.pedestrianWaiting = true;
      }
    }
};

// Init things
Pins pins{23, 25, 27, 29, 31, 33};
TrafficLightHardware hardware(pins);
TrafficLightController light;
Command command_previous {false, false, false, false, false, false};

void setup() {
  // put your setup code here, to run once:
  /* This is an expected function for the Arduino runtime. */
  light.begin();

  // initialize the hardware (pins)
  hardware.begin();

  // Tell Arduino to invoke this function whenever the following hardware event happens.
  attachInterrupt(
    digitalPinToInterrupt(PED_BUTTON_PIN),
    onPedestrianButtonPress,
    FALLING
  );
}

void loop() {
  // put your main code here, to run repeatedly:
  /* This is an expected function for the Arduino runtime.*/

  // If the pedestrian requests a light --- an interrupt --- handle that
  // by transferring that event "into the FSM"; i.e., the FSM updates its
  // own internal state. Future `update` calls behave differently because
  // state has now changed.
  if (pedestrianRequested) {
    pedestrianRequested = false;
    light.requestPedestrianCrossing();
  }

  // 
  Command command_current = light.update(millis());

  // Update hardware if the command has changed
  if (!(command_current == command_previous)) {
    hardware.applyCommand(command_current);
    command_previous = command_current;
  }
}


/* The sketch is basically wrapped into something like
int main() {
  init(); // Arduino core initialization
  setup();
  while (true) {
    loop();
  }
}
*/