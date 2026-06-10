/* Lab 7: PWM.

  https://docs.arduino.cc/learn/microcontrollers/analog-output/

  This is a simple way to generate analog-like output from a digital system
  (contrast it with using a DAC, which is more complex).

  Instead of outputting a true analog signal, it outputs a digital signal but for
  a fraction of time. As you change the pulse width, it often looks like an
  analog signal. PWM is always switching very fast between HIGH and LOW; it looks
  analog because of the average.

  E.g., if we want a 2.5 V signal, the duty cycle (on-time) will be 50%.

  Key: PWM works when the pulse switching is *much faster* than the system's response time.
  If PWM switches as 490 - 980 Hz, then the period is around 1-2 ms.
  LEDs respond in microseconds, but eyes are perceptually slow so it works.
  Motors have mechanical inertia and take ms to seconds to respond, so it works.

  Extra info: the Arduino's PWM happens entirely in hardware. When we do analogWrite(PIN, 128),
  it writes that to a hardware timer register. The microcontroller's timer peripheral compares
  this number against the current time value to determine how to set the output as LOW or HIGH.
*/

// uint8_t LED_PIN = 12;
// const float freq = 0.5;   // Hz

// // I will make my LED brightness oscillate sinusoidally
// int signal(float t_s) {
//   float x = 128 + 127 * sin(2 * PI * freq * t_s);
//   return (int)x;
// }

// void setup() {
//   pinMode(LED_PIN, OUTPUT);
// }

// void loop() {
//   analogWrite(LED_PIN, signal(millis() / 1000.0));
// }


/* Lab 7B: Potentiometer controls brightness.

  This is fairly straightforward. I read the value of a potentiometer, then
  use that value to set the output duty cycle on an LED.
*/

uint8_t LED_PIN = 12;
uint8_t POT_PIN = A0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(POT_PIN, INPUT);
}

// void loop() {
//   int pot_val = analogRead(POT_PIN);
//   float margin = 20.0; // Just to stop it from maxxing out

//   // mapping 0-1023 -> 20-235
//   float out_val = margin + ((255.0 - margin) / 1023.0) * pot_val;
  
//   analogWrite(LED_PIN, out_val);
// }
void loop() {
  int pot = analogRead(A0);

  float x = pot / 1023.0;        // 0..1
  float y = pow(x, 2.2);         // gamma correction

  int pwm = (int)(y * 255.0);

  analogWrite(12, pwm);
}





