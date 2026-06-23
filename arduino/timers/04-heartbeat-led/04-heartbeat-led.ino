// void setup_Timer1(){
//   // Clear Timer1 Control Registers
//   TCCR1A = 0;
//   TCCR1B = 0;

//   // Reset counter
//   TCNT1 = 0;

//   // Set mode to CTC (Clear Timer on Compare Match)
//   TCCR1B |= (1 << WGM12);

//   // Set the prescaler to 64
//   // Why? (2^16 * prescaler) / (16 M) >= 0.1 s => prescaler >= 24.4 so must be 64
//   TCCR1B |= (1 << CS11) | (1 << CS10);
//   // Then the timer ticks at (64 / 16 M) s = 4 us/count
//   // So 100ms / 4us = 25k counts

//   // Set compare match target
//   OCR1A = 25000 - 1;

//   // Enable the interrupt on Compare Match 1
//   TIMSK1 |= (1 << OCIE1A);
// }

// void setup() {
//   pinMode(13, OUTPUT);
//   setup_Timer1();
// }

// void loop() {
// }

// ISR(TIMER1_COMPA_vect){
//   // Toggle the LED
//   PINB = (1 << PB7);
// }

/* Try this again, but use state instead of toggling the LED inside the interrupt. */

enum class LedState {
  ON1,    // 100 ms
  OFF1,   // 100 ms
  ON2,    // 100 ms
  OFF2_1, // 100 ms
  OFF2_2, // 100 ms
  OFF2_3, // 100 ms
  OFF2_4, // 100 ms
  OFF2_5, // 100 ms
  OFF2_6, // 100 ms
  OFF2_7, // 100 ms
};

volatile bool timer_tick = false;
LedState state;

void setup_Timer1(){
  cli();
  // Clear Timer1 Control Registers
  TCCR1A = 0;
  TCCR1B = 0;

  // Reset counter
  TCNT1 = 0;

  // Set mode to CTC (Clear Timer on Compare Match)
  TCCR1B |= (1 << WGM12);

  // Set the prescaler to 64
  // Why? (2^16 * prescaler) / (16 M) >= 0.1 s => prescaler >= 24.4 so must be 64
  TCCR1B |= (1 << CS11) | (1 << CS10);
  // Then the timer ticks at (64 / 16 M) s = 4 us/count
  // So 100ms / 4us = 25k counts

  // Set compare match target
  OCR1A = 25000 - 1;

  // Enable the interrupt on Compare Match 1
  TIMSK1 |= (1 << OCIE1A);
  sei();
}

void update_led_state() {
  switch (state) {
    case LedState::ON1:
      state = LedState::OFF1;
      digitalWrite(13, LOW);
      break;
    case LedState::OFF1:
      state = LedState::ON2;
      digitalWrite(13, HIGH);
      break;
    case LedState::ON2:
      state = LedState::OFF2_1;
      digitalWrite(13, LOW);
      break;
    case LedState::OFF2_1:
      state = LedState::OFF2_2;
      break;
    case LedState::OFF2_2:
      state = LedState::OFF2_3;
      break;
    case LedState::OFF2_3:
      state = LedState::OFF2_4;
      break;
    case LedState::OFF2_4:
      state = LedState::OFF2_5;
      break;
    case LedState::OFF2_5:
      state = LedState::OFF2_6;
      break;
    case LedState::OFF2_6:
      state = LedState::OFF2_7;
      break;
    case LedState::OFF2_7:
      state = LedState::ON1;
      digitalWrite(13, HIGH);
      break;
    default:
      break;
  }
}
void setup() {
  pinMode(13, OUTPUT);
  // initialize state
  state = LedState::ON1;
  update_led_state();

  setup_Timer1();
}

void loop() {
  if (timer_tick) {
    timer_tick = false; // technically a race condition
    update_led_state();
  }
}

ISR(TIMER1_COMPA_vect){
  // Set global flag to indicate that an interrupt occurred
  timer_tick = true;
}
