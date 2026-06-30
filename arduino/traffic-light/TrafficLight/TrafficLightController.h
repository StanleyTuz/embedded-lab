#pragma once
#include "Types.h"

class TrafficLightController {
    private:
        FSMState state;

    public:
        TrafficLightController();

        void begin();

        void update_state(unsigned long now);

        Command produceCommand();

        void requestPedestrianCrossing();
};