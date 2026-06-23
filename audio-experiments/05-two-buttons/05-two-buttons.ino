#define SPEAKER_PIN 11  // PB7
#define BUTTON_A_PIN 4
#define BUTTON_B_PIN 5
#define FREQ_A 440
#define FREQ_B 880

uint16_t get_hpt(uint16_t frequency) {
  return F_CPU / (2UL * frequency);
}

enum class State{
  NONE, A, B, AB
};

bool is_sounding;
State state;

void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  pinMode(SPEAKER_PIN, OUTPUT);
  pinMode(BUTTON_A_PIN, INPUT_PULLUP);
  pinMode(BUTTON_B_PIN, INPUT_PULLUP);
  is_sounding = false;

  state = State::NONE;

  // set up the timer
  TCCR1A = 0; // timer/counter control register
  TCCR1B = 0; // timer/counter control register

  // intialize timer count to zero
  TCNT1 = 0;

  // Set timer to CTC mode
  TCCR1B |= (1 << WGM12);
  // What do we do on every cycle through? We toggle the pin
  TCCR1A &= ~(1 << COM1A0); // start with pin disconnected

  // Set prescaler=1
  TCCR1B |= (1 << CS10);

  // Set Output Compare Register (16-bits/2-bytes)
  OCR1A = 100 - 1;
}


void read_buttons() {
  bool a = (digitalRead(BUTTON_A_PIN) == LOW);
  bool b = (digitalRead(BUTTON_B_PIN) == LOW);
  
  if (!a && !b) {
    state = State::NONE;
  } else if (a && b) {
    state = State::AB;
  } else if (a) {
    state = State::A;
  } else {
    state = State::B;
  }
}

void disable_sound() {
  TCCR1A &= ~(1 << COM1A0); // disconnect OC1A from the pin
  digitalWrite(SPEAKER_PIN, LOW);
  is_sounding = false;
}

void play_freq(uint16_t frequency) {
  TCCR1A |= (1 << COM1A0);
  OCR1A = get_hpt(frequency) - 1;
  is_sounding = true;
}

void update_output() {
  switch (state) {
    case State::NONE:
      disable_sound();
      break;
    case State::A:
      play_freq(FREQ_A);
      break;
    case State::B:
    case State::AB:
      play_freq(FREQ_B);
      break;
  }
}

void loop() {
  // Read the button pin
  read_buttons(); // sets state
  update_output(); // change what note emits
}
