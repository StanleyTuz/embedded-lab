#define SPEAKER_PIN 11  // PB7
#define POT_PIN A0
#define UPDATE_DELAY_MS 1
constexpr uint16_t ADC_BUCKET_SIZE = 79; // ~= 1023 / 13 

unsigned long last_update_ms;
unsigned long now;
uint8_t knob_index;


const uint16_t half_period_ticks[] = { 
  18181, 17161, 16198, 15289, 14430, 13620,
  12856, 12134, 11453, 10810, 10204, 9631,
  9090
};

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
  uint16_t pot_val = analogRead(A0);  // 0 - 1023
  knob_index = (int)(1023 / pot_val);
  OCR1A = half_period_ticks[knob_index] - 1;
}

void loop() {
  now = millis();
  if (now - last_update_ms > UPDATE_DELAY_MS){
    last_update_ms = now;
    // Read the value of a potentiometer and set the frequency.
    knob_index = analogRead(A0) / ADC_BUCKET_SIZE;
    OCR1A = half_period_ticks[knob_index] - 1;
  }
}
