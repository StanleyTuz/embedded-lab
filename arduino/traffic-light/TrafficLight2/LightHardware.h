#include <Arduino.h>

class LightHardware {
    private:
        uint8_t pin_r, pin_y, pin_g;
    public:
        LightHardware(uint8_t pin_r, uint8_t pin_y, uint8_t pin_g)
            : pin_r(pin_r), pin_y(pin_y), pin_g(pin_g) {}

        void begin() {
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