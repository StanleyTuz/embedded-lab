#include "TrafficLightController.h"
#include "TrafficLightHardware.h"
#include <Arduino.h> // to silence intellisense

#define PED_BUTTON_PIN 2

Pins pins{23, 25, 27, 29, 31, 33};

TrafficLightController controller;
TrafficLightHardware hardware(pins);

Command previousCommand{false, false, false, false, false, false};


// Interrupt 
volatile bool pedestrianRequested = false;
void onPedestrianButtonPress() {
  /* Callback AKA Interrupt Service Routine (ISR) AKA interrupt handler.
  
  This is a "callback" because it is a function that is passed as data to
  another function (attachInterrupt) which calls it later (callback pattern).
  */
  // Set the global flag to true so that the controller sees it
  pedestrianRequested = true;
}


void setup() {
  // put your setup code here, to run once:
  /* This is an expected function for the Arduino runtime. */
  controller.begin();
  hardware.begin(); // initialize the hardware (pins)

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
    controller.requestPedestrianCrossing();
  }

  // Mutate internal FSM state
  controller.update_state(millis());
  // Map current state to outputs
  Command currentCommand = controller.produceCommand();

  // Update hardware if the command has changed
  if (!(currentCommand == previousCommand)) {
    hardware.applyCommand(currentCommand);
    previousCommand = currentCommand;
  }
}
