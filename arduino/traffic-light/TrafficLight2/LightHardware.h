#include <Arduino.h>

class LightHardware {
    private:
        int pin_r, pin_y, pin_g;
    public:
        LightHardware(int pin_r, int pin_y, int pin_g)
            : pin_r(pin_r), pin_y(pin_y), pin_g(pin_g) {
                pinMode(pin_r, OUTPUT);
                pinMode(pin_y, OUTPUT);
                pinMode(pin_g, OUTPUT);
            }
    
        void turn_red() {
            digitalWrite(pin_r, HIGH);
            digitalWrite(pin_y, LOW);
            digitalWrite(pin_g, LOW);
        };

        void turn_yellow() {
            digitalWrite(pin_r, LOW);
            digitalWrite(pin_y, HIGH);
            digitalWrite(pin_g, LOW);
        };

        void turn_green() {
            digitalWrite(pin_r, LOW);
            digitalWrite(pin_y, LOW);
            digitalWrite(pin_g, HIGH);
        };
};