#define SPEAKER_PIN 11  // PB7
#define POT_PIN A0
#define UPDATE_DELAY_MS 1

// uint16_t frequency = 440;
// uint16_t half_period_us = (int)(1000000 * (0.5/frequency));
uint16_t half_period_ticks;
unsigned long last_update_ms;
unsigned long now;

void set_half_period_ticks(uint16_t frequency) {
  half_period_ticks = F_CPU / (2UL * frequency);
  OCR1A = half_period_ticks - 1;
}


void setup() {
  // put your setup code here, to run once:
  pinMode(POT_PIN, INPUT);
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
  set_half_period_ticks(440);
  OCR1A = half_period_ticks - 1;
}

void loop() {
  now = millis();
  if (now - last_update_ms > UPDATE_DELAY_MS){
    last_update_ms = now;
    // Read the value of a potentiometer and set the frequency.
    uint16_t frequency = (int)(440*(1.0 + analogRead(A0) / 1023.0));
    set_half_period_ticks(frequency);
  }
}
