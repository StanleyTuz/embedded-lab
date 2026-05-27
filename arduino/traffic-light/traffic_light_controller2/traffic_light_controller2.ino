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


enum State {
  A_GREEN,  // Light A is green
  A_YELLOW, // Light A is yellow
  BOTH_RED_TO_B, // Both lights are red, B is about to turn green
  B_GREEN,  // Light B is green
  B_YELLOW,  // Light B is yellow
  BOTH_RED_TO_A // Both lights are red, A is about to turn green
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


class TrafficLightController {
  private:
    State state;
    State previousState;
    unsigned long stateStartTime;
    bool pedestrianWaiting; // a request exists that hasn't yet been handled (event latch)

    // pin assignments
    int aRed, aYellow, aGreen;
    int bRed, bYellow, bGreen;

  public:
    // constructor
    TrafficLightController(int ar, int ay, int ag, int br, int by, int bg)
      : aRed(ar), aYellow(ay), aGreen(ag),
        bRed(br), bYellow(by), bGreen(bg),
        state(A_GREEN),
        previousState(A_GREEN),
        stateStartTime(millis()),
        pedestrianWaiting(false) {}

    void begin() {
      pinMode(aRed, OUTPUT);
      pinMode(aYellow, OUTPUT);
      pinMode(aGreen, OUTPUT);
      pinMode(bRed, OUTPUT);
      pinMode(bYellow, OUTPUT);
      pinMode(bGreen, OUTPUT);
      pinMode(PED_BUTTON_PIN, INPUT_PULLUP);

      // Tell Arduino to invoke this function whenever the following hardware event happens.
      attachInterrupt(
        digitalPinToInterrupt(PED_BUTTON_PIN),
        onPedestrianButtonPress,
        FALLING
      );
    }

    void update() {
      // Contains state-switching logic
      unsigned long now = millis();
      
      switch (state) {
        // If A is green and it's time to turn yellow
        case A_GREEN:
          if ((now - stateStartTime > GREEN_TIME_MS)
            || pedestrianWaiting ) {
            // edge-triggered consumption
            pedestrianWaiting = false;
            state = A_YELLOW;
            stateStartTime = now;
          }
          break;
        case A_YELLOW:
          // If A has been yellow long enough, A turns red
          // in preparation of B turning green
          if (now - stateStartTime > YELLOW_TIME_MS) {
            state = BOTH_RED_TO_B;
            stateStartTime = now;
          }
          break;
        case BOTH_RED_TO_B:
          // If both are red and B is ready to turn on
          if (now - stateStartTime > BOTH_RED_TIME_MS) {
            state = B_GREEN;
            stateStartTime = now;
          }
          break;
        case B_GREEN:
          if (now - stateStartTime > GREEN_TIME_MS) {
            state = B_YELLOW;
            stateStartTime = now;
          }
          break;
        case B_YELLOW:
          if (now - stateStartTime > YELLOW_TIME_MS) {
            state = BOTH_RED_TO_A;
            stateStartTime = now;
          }
          break;
        case BOTH_RED_TO_A:
          if (now - stateStartTime > BOTH_RED_TIME_MS) {
            state = A_GREEN;
            stateStartTime = now;
          }
          break;
      }
      if (state != previousState) {
        applyStateChange();
      }
    }

    void applyStateChange() {
      /* Toggle LEDs based on state. */
      previousState = state;

      bool aTurnGreen = (state == A_GREEN);
      bool aTurnYellow = (state == A_YELLOW);

      bool bTurnGreen = (state == B_GREEN);
      bool bTurnYellow = (state == B_YELLOW);

      digitalWrite(aGreen, aTurnGreen);
      digitalWrite(aYellow, aTurnYellow);
      digitalWrite(aRed, !(aTurnGreen || aTurnYellow));
      digitalWrite(bGreen, bTurnGreen);
      digitalWrite(bYellow, bTurnYellow);
      digitalWrite(bRed, !(bTurnGreen || bTurnYellow));
    }

    // button press -> set flag True -> stay True until consumed (latched request model)
    // OTHER: button press is only meaningful at certain times, otherwise it's ignored (edge-gated event model)
    void requestPedestrianCrossing() {
      // When a pedestrian requests a crossing, the controller stores
      // the event internally

      // Note: This is making it a edge-(or state-)gated model instead of a pure latched model (persistent request model)
      // The input function depends on the current state now; "input semantics have become state-dependent"
      if (state == A_GREEN) {
        pedestrianWaiting = true;
      }
    }
};

TrafficLightController light(23, 25, 27, 29, 31, 33);

void setup() {
  // put your setup code here, to run once:
  light.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

  // If the pedestrian requests a light --- an interrupt --- handle that
  // by transferring that event "into the FSM"; i.e., the FSM updates its
  // own internal state. Future `update` calls behave differently because
  // state has now changed.
  if (pedestrianRequested) {
    pedestrianRequested = false;
    light.requestPedestrianCrossing();
  }

  light.update();
}
