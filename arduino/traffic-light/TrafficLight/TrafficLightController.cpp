#include "TrafficLightController.h"

static constexpr unsigned long GREEN_TIME_MS = 5000;
static constexpr unsigned long YELLOW_TIME_MS = 2000;
static constexpr unsigned long BOTH_RED_TIME_MS = 1000;

TrafficLightController::TrafficLightController()
    : state{State::A_GREEN, 0, false} {}

void TrafficLightController::begin() {
    state.startTime = 0;
}

void TrafficLightController::update_state(unsigned long now) {
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
}

Command TrafficLightController::produceCommand() {
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

void TrafficLightController::requestPedestrianCrossing() {
    if (state.currentState == State::A_GREEN) {
        state.pedestrianWaiting = true;
    }
}