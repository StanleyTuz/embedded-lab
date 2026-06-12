/*
  In the previous version, on every iteration of loop() we
  - read the potentiometer value; and
  - updated the output value of the oscillator (phase).
  Thus, the sample rate in this case is exactly how often the loop runs.

  In this version:
  - The potentiometer is still read every loop() iteration; but
  - the oscillator output runs in a hardware timer interrupt, not in loop().
  This means that the oscillator output happens at a fixed rat. The ISR runs
  at precise intervals, and generates one audio sample per tick.
*/

float phase = 0.0;

// Each loop iteration is treated as one audio sample
// Thus we will use delay() in the loop to approximately
// force the sample rate.
constexpr float SAMPLE_RATE = 10000.0;

long count = 0;
void setup() {
  pinMode(13, OUTPUT);

  /* --- Set up hardware timer to use for sampling --- */
  cli(); // temporarily disable interrupts

  TCCR1A = 0; // normal mode
  TCCR1B = 0;
  TCNT1 = 0;  // reset counter

  // compare match value (controls interrupt rate)
  // formula: 16MHz / (prescaler * (OCR1A + 1))
  OCR1A = 1599; // with prescaler=1, gives ~10kHz

  // CTC mode (clear timer on compare match)
  TCCR1B |= (1 << WGM12);
  // Set prescaler = 1
  TCCR1B |= (1 << CS10);
  // enable interrupt on compare match A
  TIMSK1 |= (1 << OCIE1A);

  sei(); // re-enable the inputs
}

void loop() {
  // Slow control stuff goes here
  // Read control input (potentiometer)
  int pot = analogRead(A0);
  float frequency = map(pot, 0, 1023, 50, 2000);
}

ISR(TIMER1_COMPA_vect) {
  count++;

  // sample rate is 10kHz, so if we toggle every 5000 samples,
  // then we get LED state changing every 5000/10k = 0.5 seconds.
  // Thus the LED blinks with a period of 1 sec, or freq of 1 Hz.
  if (count >= 5000) {
    count = 0;
    // toggle pin at interrupt rate (testing)
    digitalWrite(13, !digitalRead(13));
  }
}