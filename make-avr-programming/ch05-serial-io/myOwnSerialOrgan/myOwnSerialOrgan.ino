#define SPEAKER_PIN 28 

uint32_t half_period_us;
uint32_t duration_us;

void playNote(uint32_t half_period_us, uint32_t duration_us) {
  uint32_t elapsed_us;
  uint32_t i;

  // one iteration per half-period
  for (elapsed_us = 0; elapsed_us < duration_us; elapsed_us += half_period_us) {
    for (i = 0; i < half_period_us; i++ ){
      _delay_us(1);
    }
    digitalWrite(SPEAKER_PIN, !digitalRead(SPEAKER_PIN));
  }
}

void setup() {
  pinMode(SPEAKER_PIN, OUTPUT);

  uint16_t frequency = 440;
  half_period_us = (int)((1000000.0/2.0) / frequency);
  // half_period_us = 1043;
  duration_us = 1000000;
}

void loop() {
  playNote(half_period_us, duration_us);
  delay(1000);
}