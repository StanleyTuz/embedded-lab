#include "IntersectionController.h"

static constexpr unsigned long GREEN_TIME_MS = 5000;
static constexpr unsigned long YELLOW_TIME_MS = 2000;
static constexpr unsigned long BOTH_RED_TIME_MS = 1000;

LightStates IntersectionController::get_light_states() const {
    switch (state) {
        case IntersectionState::NS_GREEN_EW_RED:
            return {TrafficLightState::GREEN, TrafficLightState::RED};
        case IntersectionState::NS_YELLOW_EW_RED:
            return {TrafficLightState::YELLOW, TrafficLightState::RED};
        case IntersectionState::NS_RED_EW_GREEN:
            return {TrafficLightState::RED, TrafficLightState::GREEN};
        case IntersectionState::NS_RED_EW_YELLOW:
            return {TrafficLightState::RED, TrafficLightState::YELLOW};
        case IntersectionState::BOTH_RED_TO_EW:
        case IntersectionState::BOTH_RED_TO_NS:
        default:
            return {TrafficLightState::RED, TrafficLightState::RED};
    }
}


void IntersectionController::update_state(unsigned long now) {
    // Contains state-switching logic

    switch (state) {
        // If NS is green and it's time to turn yellow
        case IntersectionState::NS_GREEN_EW_RED:
            if ((now - stateStartTime > GREEN_TIME_MS)
                || pedestrianWaiting ) {
                // edge-triggered consumption
                pedestrianWaiting = false;
                state = IntersectionState::NS_YELLOW_EW_RED;
                stateStartTime = now;
            }
            break;
        case IntersectionState::NS_YELLOW_EW_RED:
            // If NS has been yellow long enough, NS turns red
            // in preparation of EW turning green
            if (now - stateStartTime > YELLOW_TIME_MS) {
                state = IntersectionState::BOTH_RED_TO_EW;
                stateStartTime = now;
            }
            break;
        case IntersectionState::BOTH_RED_TO_EW:
            // If both are red and EW is ready to turn on
            if (now - stateStartTime > BOTH_RED_TIME_MS) {
                state = IntersectionState::NS_RED_EW_GREEN;
                stateStartTime = now;
            }
            break;
        case IntersectionState::NS_RED_EW_GREEN:
            if (now - stateStartTime > GREEN_TIME_MS) {
                state = IntersectionState::NS_RED_EW_YELLOW;
                stateStartTime = now;
            }
            break;
        case IntersectionState::NS_RED_EW_YELLOW:
            if (now - stateStartTime > YELLOW_TIME_MS) {
                state = IntersectionState::BOTH_RED_TO_NS;
                stateStartTime = now;
            }
            break;
        case IntersectionState::BOTH_RED_TO_NS:
            if (now - stateStartTime > BOTH_RED_TIME_MS) {
                state = IntersectionState::NS_GREEN_EW_RED;
                stateStartTime = now;
            }
            break;
    }
}


void IntersectionController::requestPedestrianCrossing() {
    // This button only works if the NS light is green.
    if (state == IntersectionState::NS_GREEN_EW_RED) {
        pedestrianWaiting = true;
    }
}