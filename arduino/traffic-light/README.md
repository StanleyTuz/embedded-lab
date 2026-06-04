# Overview

This project is
- A non-blocking traffic light program (FSM) on an Arduino.
- Includes interrupt-driven pedestrian input.
- Code is structured to separate the control logic from hardware.
- A very small test harness.

# FSM specification

This is a "contract"

States:
- `A_GREEN`
- `A_YELLOW`
- `BOTH_RED_TO_B`
- `B_GREEN`
- `B_YELLOW`
- `BOTH_RED_TO_A`

Transition rules
- `A_GREEN` ->

# Timing model

- No blocking delays
- Time is measured using `millis()`
- FSM is poll-based

# Event model

There are three event types:
- Interrupt event: a button press turns on a volatile flag
- Latched event: a persistent boolean flag set by an interrupt, cleared by FSM consumption.
- Time event: lights change after time thresholds have been crossed.

# Testing

Currently tested:
- State transitions
- Command outputs
- Time-based transitions

Not currently tested:
- Hardware timing accuracy
- ISR correctness under race conditions
- Real electrical behavior.

A very basic unit test suite is implemented at `tests/`. To compile the test function, run
```bash
g++ tests/test_controller.cpp \
    TrafficLight/TrafficLightController.cpp \
    -I TrafficLight \
    -o tests/test.exe
```
Then run the program with
```bash
./tests/test
```

# Hardware abstraction

The `TrafficLightHardware` is a hardware abstraction layer (HAL) that isolates object isolates GPIO operations (e.g., `pinMode`, `digitalWrite`) from the FSM logic.

# Design decisions/tradeoffs

- Chose a polling loop instead of interrupts for determinism simplicity
- ISR only sets a flag to avoid heavier interrupt work
- FMS is not fully re-entrant/not thread-safe
- Output comparison is added to reduce redundant writes

# Known limitations

- Not real-time deterministic
- No debouncing on input
- Race condition risk on volatile bool
- FSM not formally verified
- No scheduler abstraction