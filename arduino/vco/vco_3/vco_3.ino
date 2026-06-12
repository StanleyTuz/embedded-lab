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

// volatile float phase = 0.0;
// volatile float phase_increment = 0.0;
volatile uint32_t phase = 0;
volatile uint32_t phase_increment = 0;
volatile uint8_t signal = 0;   // FIX: must be volatile + 8-bit for ISR/PWM stability

// Each loop iteration is treated as one audio sample
// Thus we will use delay() in the loop to approximately
// force the sample rate.
constexpr float SAMPLE_RATE = 8000.0;  // 8kHz

long count = 0;

void setup() {
  pinMode(10, OUTPUT);

  /* --- Set up hardware timer to use for sampling --- */
  cli(); // temporarily disable interrupts

  TCCR1A = 0; // normal mode
  TCCR1B = 0;
  TCNT1 = 0;  // reset counter

  // compare match value (controls interrupt rate)
  // formula: 16MHz / (prescaler * (OCR1A + 1))
  OCR1A = 1999; // with prescaler=1, gives ~8kHz

  // CTC mode (clear timer on compare match)
  TCCR1B |= (1 << WGM12);
  // Set prescaler = 1
  TCCR1B |= (1 << CS10);
  // enable interrupt on compare match A
  TIMSK1 |= (1 << OCIE1A);

  // Fast PWM on Timer2
  TCCR2A = (1 << WGM20) | (1 << WGM21) | (1 << COM2A1);
  TCCR2B = (1 << CS20); // no prescaler

  sei(); // re-enable interrupts
}

// void loop() {
//   // Slow control stuff goes here
//   // Read control input (potentiometer)
//   float frequency = 50.0 + (analogRead(A1)) * (1950.0 / 1023.0);

//   noInterrupts();

//   // FIX: correct 32-bit phase increment scaling (no truncation bug)
//   phase_increment = (uint32_t)((frequency * 4294967296.0) / SAMPLE_RATE);

//   interrupts();
// }
void loop() {
  int pot = analogRead(A1);
  float norm = pot / 1023.0;

  // smooth control (removes jitter)
  static float smooth = 0.0;
  smooth = smooth * 0.9 + norm * 0.1;

  // logarithmic frequency range
  float frequency = 50.0 * pow(2000.0 / 50.0, smooth);

  noInterrupts();
  phase_increment =
    (uint32_t)((frequency * 4294967296.0) / SAMPLE_RATE);
  interrupts();
}

ISR(TIMER1_COMPA_vect) {
  /* When interrupt occurs
  - increment the phase
  - compute the waveform amplitude from the phase
  - write the amplitude to a PWM-capable output pin.
  */

  // Increment the phase. Each step of the timer takes time 1 / SAMPLE_RATE
  phase += phase_increment;

  // FIX: correct sawtooth extraction from fixed-point phase
  signal = phase >> 24;

  // Write to a PWM pin
  OCR2A = signal;
}