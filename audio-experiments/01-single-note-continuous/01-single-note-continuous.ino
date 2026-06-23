/* This produces something _close_ to a 440 Hz square wave (A4), but
there will be inconsistencies due to imprecise timing: the _delay_us() call,
the overhead of the for-loop, the pin toggle time, etc.
*/

// #define SPEAKER_PIN 28

// uint16_t frequency = 440;
// uint16_t half_period_us = (int)(1000000 * (0.5/frequency));

// void setup() {
//   // put your setup code here, to run once:
//   pinMode(SPEAKER_PIN, OUTPUT);
// }

// void loop() {
//   // put your main code here, to run repeatedly:
//   for (uint16_t i = 0; i < half_period_us; i++) {
//     _delay_us(1);
//   }
//   // toggle the speaker pin
//   digitalWrite(SPEAKER_PIN, !digitalRead(SPEAKER_PIN));
// }

/* Try again, but use a (hardware) timer to more precisely time the wave.*/
#define SPEAKER_PIN 11  // PB7

uint16_t frequency = 440;
uint16_t half_period_us = (int)(1000000 * (0.5/frequency));
uint16_t half_period_ticks = F_CPU / (2UL * frequency);

void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);

  pinMode(SPEAKER_PIN, OUTPUT);

  // set up the timer
  TCCR1A = 0; // timer/counter control register
  TCCR1B = 0; // timer/counter control register

  // Set timer to CTC mode
  TCCR1B |= (1 << WGM12);
  // What do we do on every cycle through? We toggle the pin
  TCCR1A |= (1 << COM1A0); // toggle OC1A pin on compare match (PB7)

  // Set prescaler=1
  TCCR1B |= (1 << CS10);

  // Set Output Compare Register (16-bits/2-bytes)
  OCR1A = half_period_ticks - 1;
}

void loop() {
  // Flash an LED just to prove that the sound works
  delay(300);
  digitalWrite(13, !digitalRead(13));
}
