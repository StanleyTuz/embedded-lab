#define SPEAKER_PIN 11  // PB7
#define BUTTON_PIN 4

uint16_t frequency = 440;
uint16_t half_period_us = (int)(1000000 * (0.5/frequency));
uint16_t half_period_ticks = F_CPU / (2UL * frequency);
bool is_sounding;

void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  is_sounding = false;

  pinMode(SPEAKER_PIN, OUTPUT);


  // set up the timer
  TCCR1A = 0; // timer/counter control register
  TCCR1B = 0; // timer/counter control register

  // intialize timer count to zero
  TCNT1 = 0;

  // Set timer to CTC mode
  TCCR1B |= (1 << WGM12);
  // What do we do on every cycle through? We toggle the pin
  TCCR1A &= ~(1 << COM1A0); // toggle OC1A pin on compare match (PB7)

  // Set prescaler=1
  TCCR1B |= (1 << CS10);

  // Set Output Compare Register (16-bits/2-bytes)
  OCR1A = half_period_ticks - 1;
}

void loop() {
  // Read the button pin
  if (digitalRead(BUTTON_PIN) == LOW) {
    if (!is_sounding) {
      TCCR1A |= (1 << COM1A0); // reconnect OC1A to the pin
      is_sounding = true;
    }
  }
  else {
    if (is_sounding) {
      TCCR1A &= ~(1 << COM1A0); // disconnect OC1A from the pin
      digitalWrite(SPEAKER_PIN, LOW);
      is_sounding = false;
    }
  }
}
