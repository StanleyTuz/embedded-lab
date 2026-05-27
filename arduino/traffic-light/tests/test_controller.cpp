#include <iostream>
#include "TrafficLightController.h"

void assert_true(bool condition, const char* msg) {
    if (!condition) {
        std::cout << "FAIL: " << msg << "\n";
        std::exit(1);
    }
}

int main() {
    TrafficLightController c;
    c.begin();

    // initial state
    c.update_state(0);
    auto cmd = c.produceCommand();
    assert_true(cmd.aGreen, "A should start green");

    // force time forward
    c.update_state(6000);
    cmd = c.produceCommand();

    assert_true(cmd.aYellow, "A should turn yellow after 5s");

    std::cout << "ALL TESTS PASSED\n";
}