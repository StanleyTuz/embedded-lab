/* I want to modify the Controller class so that it isn't emitting LED states anymore, but states of
each of the two traffic lights. This will be nicely scalable to, say, 3-way intersections, since
all that will need to change is the Controller.
*/
#include "IntersectionController.h"
#include "HardwareController.h"
#include <Arduino.h> // to silence intellisense

#define PED_BUTTON_PIN 2

LightHardware ns_light {23, 25, 27};
LightHardware ew_light {29, 31, 33};
IntersectionController controller;
HardwareController hardware(ns_light, ew_light, PED_BUTTON_PIN);

LightStates previous;
LightStates current;


// Interrupt configuration
volatile bool pedestrianRequested = false;
void onPedestrianButtonPress() {
  /* ISR for pedestrian button */
  pedestrianRequested = true; // set global flag
}


void setup() {
  controller.begin(millis()); // initialize the controller
  hardware.begin(); // initialize the hardware (pins)

  // Attach the pedestrian button interrupt
  attachInterrupt(
    digitalPinToInterrupt(PED_BUTTON_PIN),
    onPedestrianButtonPress,
    FALLING
  );

  previous = controller.get_light_states();
}

void loop() {
  // Main program loop.


  /* If the pedestrian requests a light --- an interrupt --- handle that
  by transferring that event "into the FSM"; i.e., the FSM updates its
  own internal state. Future `update` calls behave differently because
  state has now changed.
  */
  if (pedestrianRequested) {
    pedestrianRequested = false;  // reset global flag
    controller.requestPedestrianCrossing();
  }

  // Mutate internal FSM state
  controller.update_state(millis());
  // Map current state to outputs
  current = controller.get_light_states();

  // Update outputs if the command has changed
  if (!(current == previous)) {
    previous = current;
    hardware.apply_light_states(current);
  }
}
