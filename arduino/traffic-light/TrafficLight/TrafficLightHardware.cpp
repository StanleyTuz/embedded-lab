#include "TrafficLightHardware.h"
#include <Arduino.h>

TrafficLightHardware::TrafficLightHardware(const Pins& pins)
  : pins(pins) {}


void TrafficLightHardware::begin() {
      /* Initialize hardware by setting the pin output modes. */
      pinMode(pins.aRed, OUTPUT);
      pinMode(pins.aYellow, OUTPUT);
      pinMode(pins.aGreen, OUTPUT);
      pinMode(pins.bRed, OUTPUT);
      pinMode(pins.bYellow, OUTPUT);
      pinMode(pins.bGreen, OUTPUT);

      // Pedestrian button pin
      pinMode(2, INPUT_PULLUP);
    }

void TrafficLightHardware::applyCommand(const Command& out) {
      /* Given output */
      digitalWrite(pins.aRed, out.aRed);
      digitalWrite(pins.aYellow, out.aYellow);
      digitalWrite(pins.aGreen, out.aGreen);
      digitalWrite(pins.bRed, out.bRed);
      digitalWrite(pins.bYellow, out.bYellow);
      digitalWrite(pins.bGreen, out.bGreen);
    }