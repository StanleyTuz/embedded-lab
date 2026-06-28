## Goal

Build a simple MCU-based step sequencer.

Ideally I will replicate the behavior of [the Erica Synths/Moritz Klein EDU DIY analog sequencer module](https://www.ericasynths.lv/edu-diy-sequencer-2131/).

## Outline

I am going to proceed in steps, from simple to more complex. I am decomposing this into smaller "stories" because I think it makes sense

### 1. Get an Arduino-style sequencer working

1. Implement a basic working software version of the sequencer core.
    - Lean heavily on the Arduino API.
    - Use fixed control parameters like step rate and the steps' values.
    - Do timing in the main loop via polling.
    - Output step values with PWM.
2. Add a potentiometer knob for step rate.
3. Add a push-button to cycle through 3-step, 4-step, and 5-step modes.
    - Note: need to add state here. I'll probably implement `StepMode` to manage this.
    - Also need to debounce the button press so it feels better.
4. Add pots for the step levels.

### 2. Make it adhere to synth module standards

1. Switch to outputting analog via a DAC (instead of PWM).
2. Implement a scaling/output stage.
    - Take the output of the sequencer and
3. Demonstrate "musicality" by testing it with a VCO (or some software synth).

### 3. Lower-level optimization

Forgo the Arduino API in favor of AVR C.

1. Implement timer/interrupt for the step rate timing (instead of polling with `millis`).
2. Configure low-level PWM (instead of `analogWrite`).

### 4. Migrate to a different MCU

I have a raw ATmega168 chip. I want to migrate this sequencer to that chip.

1. Suitably modify the code.
    - Since the 168 has enough peripheral channels (PWM and ADC), this should mostly be an exercise in re-mapping the pins.
    - There is 16Kb of Flash on the 168, as compared with the 256Kb on the 2560. The program itself is very small, so this shouldn't be a big issue.
2. Use my Arduino Mega as an ISP to program the 168 chip (I don't have a hardware programmer).