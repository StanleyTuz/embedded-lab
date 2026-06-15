/* Goal: Toggle on-board LED every 100 ms.

I know that the clock is 16 MHz, and the timers are 16 bits wide.
So I need a prescaler such that
(2^16 counts) * prescaler [cycles/count] / (16M cycles/sec) >= 100 ms.
Prescaler=64 works.

With this prescaler, each count is 64 cycles, so each count is
(64 cycles/count) / (16M cycles / sec) = 4 us / count.

Thus for 100 ms, we need
100 ms / (4 us/ count) = 25k counts
So we will do a compare match interrupt at 25k-1.

Timer1 is a general-purpose timer, so we will use that.
*/

void setup_Timer1() {
  cli(); // disable interrupts during setup

  // Clear Timer1 control registers.
  //  These both configure how Timer1 behaves. There are two because one register isn't enough
  //  to hold all the configuration options. Like different "config pages" for the same hardware timer.
  //   TCCR1A = Timer/Counter Control Register [for] Timer1, register A (first part)
  TCCR1A = 0; // Timer 1 Control Register A
  TCCR1B = 0; // Timer 1 Control Register B

  // Reset Timer 1 counter
  // TCNT1 = Timer/Counter1 counter register
  TCNT1 = 0;  // set to 0

  // Set the compare match target ("compare value")
  // OCR1A = Output Compare Register [Timer]1 [Channel]A
  // Note: on Timer1 we also have OCR1B, OCR1C. These are different channels.
  //   Each channel can trigger its own interrupt or control output pins.
  OCR1A = 24999;  // When it reaches 24999, trigger interrupt

  // Set mode to CTC - Clear Timer on Compare Match
  // Thus, when TCNT1 == OCR1A, reset counter to 0
  // Explanation: "|=" is compound assignment operator that does bitwise logical-OR of TCCR1B with
  //  the right-hand side. So this is basically just updating some bits in the timer control register TCCR1B.
  // WGM12 is a named constant for a bit position (Waveform Generation Mode). It is like 3, 4, 5, etc.
  //  "1 << WGM12" is a bit mask generator. Take binary 00000001 and shift it left by WGM12 positions.
  //  Thus the line is setting only the bit WGM12 of TCCR1B.
  TCCR1B |= (1 << WGM12);

  // Set the prescaler to 64
  //  CS10, CS11, CS12 are each Clock Select bits in Timer1's control register B. They are bit positions.
  //  They define where the timer gets its clock from, and how much it's divided (prescaler).
  //    "1 << CS11" set bit CS11 to 1.
  //  Search the datasheet for "TCCR1B", and it shows these bits. Scroll down and there is a table titled
  //  "Clock Select Bit Description" that shows the 8 possible combinations of the three, and what they mean.
  //  So (CSn2, CSn1, CSn0) = (0, 1, 1) is set prescaler to 64
  TCCR1B |= (1 << CS11) | (1 << CS10);

  // Enable the interrupt on Compare Match A
  // This tells hardware "when TCNT1 == OCR1A, generate an interrupt"
  //  TIMSK1 = Time/Counter 1 Interrupt Mask Register
  //  OCIE1A = Timer/Counter 1, Output Compare A Match Interrupt Enable (bit position)
  //    When this bit is written to 1, the Compare A Match interrupt is enabled. 
  TIMSK1 |= (1 << OCIE1A);

  // Re-enable global interrupts
  sei();
}

void setup() {
  pinMode(13, OUTPUT);
  setup_Timer1();
}

void loop() {
  // Nothing happens here
}

ISR(TIMER1_COMPA_vect)
{
  // Toggle the LED
  digitalWrite(13, !digitalRead(13));
}