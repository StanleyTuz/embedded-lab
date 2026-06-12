

float phase = 0.0;

// Each loop iteration is treated as one audio sample
// Thus we will use delay() in the loop to approximately
// force the sample rate.
constexpr float SAMPLE_RATE = 10000.0;
constexpr float DELAY_TIME_S = 1.0 / SAMPLE_RATE;

void setup() {
  Serial.begin(115200);
}

void loop() {
  // Read control input (potentiometer)
  int pot = analogRead(A0);
  float frequency = map(pot, 0, 1023, 50, 2000);

  // Increment the phase
  float phase_increment = frequency / SAMPLE_RATE;

  // Update the phase accumulator
  phase += phase_increment;

  // Mod the phase
  if (phase >= 1.0) {
    phase -= 1.0;
  }

  // Use the phase to get a saw wave
  float osc_sample = (phase * 2.0) - 1.0;

  // Output to Serial (later to speaker)
  Serial.println(osc_sample);

  // Approximately enforce the sample rate
  delayMicroseconds(DELAY_TIME_S * 1000000);
}
