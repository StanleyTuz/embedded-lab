#pragma once
#include "Types.h"

class IntersectionController {
    private:
        IntersectionState state;
        unsigned long stateStartTime;
        bool pedestrianWaiting;

    public:
        IntersectionController()
            : state(IntersectionState::NS_GREEN_EW_RED), stateStartTime(0), pedestrianWaiting(false) {};

        void begin(unsigned long now) {
            stateStartTime = now;
            pedestrianWaiting = false;
        }

        void update_state(unsigned long now);

        LightStates get_light_states() const;

        void requestPedestrianCrossing();
};