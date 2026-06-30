#pragma once
#include "Types.h"
#include "LightHardware.h"

class HardwareController {
  /* Hardware-facing object that configures and communicates with the Arduino pins.*/
  private:
    LightHardware ns;
    LightHardware ew;
    int pedestrian_button_pin;
    
  public:
    HardwareController(LightHardware ns, LightHardware ew, int pedestrian_button_pin)
      : ns(ns), ew(ew), pedestrian_button_pin(pedestrian_button_pin) {}
    
    void begin() {
      pinMode(pedestrian_button_pin, INPUT_PULLUP);
      // begin the LightHardwares
      ns.begin();
      ew.begin();
    };

    void apply_light_states(const LightStates& ls) {
      /* Given output states of the lights,
      set the LEDs accordingly. This basically translates between the
      state as specified by the IntersectionController and the actions
      to be taken by the actual Lights' LEDs.
      */
      // Set the NS light
      switch (ls.ns) {
        case TrafficLightState::RED:
          ns.turn_red();
          break;
        case TrafficLightState::YELLOW:
          ns.turn_yellow();
          break;
        case TrafficLightState::GREEN:
          ns.turn_green();
          break;
      }
      // Set the EW light
      switch (ls.ew) {
        case TrafficLightState::RED:
          ew.turn_red();
          break;
        case TrafficLightState::YELLOW:
          ew.turn_yellow();
          break;
        case TrafficLightState::GREEN:
          ew.turn_green();
          break;
      }
    }
};