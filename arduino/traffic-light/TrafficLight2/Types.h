/* Shared data structures to be used by the TrafficLightController and
TrafficLightHardware classes.*/

#pragma once

/* State for a single traffic light. */
enum class TrafficLightState {
  RED, YELLOW, GREEN
};

struct LightStates {
  TrafficLightState ns;
  TrafficLightState ew;

  // Overloaded for comparing overall intersection light states.
  bool operator==(const LightStates& other) const {
    return ns == other.ns && ew == other.ew;
  }
};

/* State representing a two-light intersection. */
enum class IntersectionState {
  NS_GREEN_EW_RED,
  NS_YELLOW_EW_RED,
  BOTH_RED_TO_EW, // Both lights are red, EW is about to turn green
  NS_RED_EW_GREEN,
  NS_RED_EW_YELLOW,
  BOTH_RED_TO_NS  // Both lights are red, NS is about to turn green
};