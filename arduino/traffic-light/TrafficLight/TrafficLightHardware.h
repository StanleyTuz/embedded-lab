#pragma once
#include "Types.h"

class TrafficLightHardware {
  /* Hardware-facing object that configures and communicates with the Arduino pins.*/
  private:
    Pins pins;  // store the pin assignments

  public:
    TrafficLightHardware(const Pins& pins);
    
    void begin();

    void applyCommand(const Command& out);
};