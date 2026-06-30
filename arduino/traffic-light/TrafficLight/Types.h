/* Shared data structures to be used by the TrafficLightController and
TrafficLightHardware classes.*/

#pragma once

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