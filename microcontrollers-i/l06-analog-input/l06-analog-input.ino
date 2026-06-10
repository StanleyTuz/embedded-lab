/* Lab 6A: Read potentiometer.

There is an example in the docs:
https://docs.arduino.cc/built-in-examples/basics/AnalogReadSerial/
*/

// void setup() {
//   Serial.begin(9600);
// }

// void loop() {
//   int sensor_value = analogRead(A0);

//   // print out the value
//   Serial.println(sensor_value);
//   delay(1); // for stability
// }

// Open the Serial Monitor in Arduino IDE to view the value of the pot (0-1023) (2^10 i.e., 10 bits)
// Then we can linearly interpolate between [0V, +5V] to get the actual voltage value.
// So it goes: pot -> analog voltage -> ADC -> digital number

// /* Lab 6B: Create software voltmeter.

// This is nearly identical to the previous example, but now I'm converting the number
// returned by digitalRead() back to a voltage. Note that this uses the known reference
// voltage used by the Arduino board.
// */

// float reading_to_volts(int sensor_value) {
//   return sensor_value * (5.0 / 1023.0);
// }

// void setup() {
//   Serial.begin(9600);
// }

// void loop() {
//   int sensor_value = analogRead(A0);

//   // print out the value
//   Serial.println(reading_to_volts(sensor_value));
//   delay(1); // for stability
// }

// /* Lab 6C: Measure noisy signal, compute average.

// I can compute a running average by storing the latest N values in an array.
// */

// const int N = 30;
// int buffer[N];
// int idx = 0;
// long sum = 0;

// float reading_to_volts(int sensor_value) {
//   return sensor_value * (5.0 / 1023.0);
// }

// void setup() {
//   Serial.begin(9600);
// }

// void loop() {
//   int x = analogRead(A0);

//   sum -= buffer[idx];
//   buffer[idx] = x;
//   sum += x;

//   idx = (idx + 1) % N;
//   int avg = sum / N;

//   // print out the value
//   Serial.println(avg);
//   // delay(1); // for stability
// }

/* The storage and indexing is kind of tedious and expensive (maybe?).
Another simpler option is an exponential average. This just stores the
average value, and then every new sample slightly adjusts it.
*/

float ema = 0;
int sensor_value;

float reading_to_volts(int sensor_value) {
  return sensor_value * (5.0 / 1023.0);
}

void setup() {
  Serial.begin(9600);
}

void loop() {
  sensor_value = analogRead(A0);
  float v = reading_to_volts(sensor_value);
  ema = ema + (sensor_value - ema) / 20.0;
  Serial.println(ema);
}