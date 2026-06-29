# Audio Experiments

A collection of experiments around generating audio from a microcontroller. These are generally Arduino sketches, but I'm trying to get down to the AVR level.

### 07 - Direct Digital Synthesis

### 07b - Timers

A collection of timer-related explorations, which may or may not be strictly audio related. I am trying to understand the different (but fundamental) ways that timers are used when generating sound. 

#### Book examples that confused me

For reference, I will summarize here some examples from the "Make: AVR Programming Book" that are currently getting mixed up in my head.

1. (Chapter 5: Serial I/O) The author implements a simple "organ" that plays notes of different frequencies.
    1. What? **A CPU-driven square wave generator, on an arbitrary GPIO pin.**
    2. How? He has pre-computed the (half-)periods of notes in a scale. To play a particular note for a given duration, he calls a `playNote()` function from the CPU. This function manually toggles the speaker pin every half-period, for the specified duration, to play the note of the desired frequency. Time is advanced via `_delay_us(1)` until the duration is reached.
    3. Notes: this is very brute-force. Timing is all approximate, with delays, and everything is done in the CPU.
2. (Chapter 9: Intro to the Timer/Counter Hardware) The author expands on the previous example by using a timer to generate the square wave at a precise frequency.
    1. What? **A CTC mode timer-driven square wave generator, on a timer output pin.**
    2. How? The author has again pre-computed the (half-)periods of the notes, in timer ticks (not seconds!). To play a particular note for a given duration, the author sets the Output Compare Register (`OCR0A`) to the note's half-period, and sets it to toggle output mode, then just runs it until the duration is reached, and then the output pin is disabled.
    3. Notes: the generation of the audio is very precise, since the timer does it. All we need to do to play a new note is to set `OCR0A` to the desired half-period and turn on the speaker pin. Note that the half-periods are not in units of seconds, but rather in timer tick counts; that is, the author's computation of the half-periods is dependent on the CPU clock rate and the prescaler used.
3. (Chapter 10: Pulse-Width Modulation) This and the following two examples are incremental developments of PWM code.
    1. What? **A very manual PWM demonstration with a fixed PWM carrier frequency, and on an arbitrary GPIO pin.**
    2. How? The author takes an LED brightness input level $b \in [0, 255]$. Each PWM cycle is a 256-iteration for-loop, with the pin ON for the first $b$ iterations, and OFF for the last $256-b$ iterations. As he sweeps through the PWM cycle, he uses `_delay_us(20)`. Thus a single PWM cycle takes about $20\, \mu s * 256 \approx 5.12 \, ms$. Thus the PWM rate is fixed here; the corresponding frequency is about 195 Hz.
    3. Notes: This is also known as "software PWM", since the CPU is directly generating the signal. In this case, we are given a brightness value and we output (encode) it via PWM duty cycle. This is a type of DAC.
4. (Chapter 10) The second PWM example improves on the first.
    1. What? **A PWM demonstration using a timer in Fast PWM mode, with an output pin.**
    2. How? Configure a timer in Fast PWM mode, and set it to output the PWM on its pin. Then set the Output Compare Register `OCR1A` to determine the duty cycle. Change the value of this register to change the output value --- an LED's brightness.
    3. Notes: now the CPU (main loop) can focus on other things, like reading user input, and the PWM output is handled entirely by the timer and PWM peripherals.
4. (Chapter 10) The third PWM example uses timer interrupts to implement something like PWM on an arbitrary GPIO pin.
    1. What? **A timer interrupt-based implementation of PWM.**
    2. How? The author sets up a timer in Normal mode with an overflow interrupt (`TIMER0_OVF_vect`). This interrupt occurs at the start of each PWM cycle. In the ISR he sets the Output Compare Register `OCR0A` to the brightness value, so that for this PWM cycle, the duty cycle is set by this value.
5. (Chapter 13: Advanced PWM Tricks)  