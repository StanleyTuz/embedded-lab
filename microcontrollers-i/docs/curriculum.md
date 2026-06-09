# Curriculum

A good Microcontrollers I course should not be:

Learn Arduino APIs.

Nor should it be:

Read 500 pages of datasheets and assembly.

It should use the Arduino as a laboratory for learning concepts that generalize to all microcontrollers.

The goal is that after the course, you can pick up an AVR, STM32, RP2040, MSP430, etc., and recognize the same concepts appearing in different forms.

Microcontrollers I

Prerequisites

Basic programming experience
High-school algebra
Basic circuit knowledge (voltage, current, resistor, capacitor)

Hardware

Arduino Uno (or equivalent)
Breadboard
LEDs
Pushbuttons
Potentiometer
Servo
Optional: I2C OLED display

## Unit 0: What Is a Microcontroller?

### Concepts
- CPU
- RAM
- Flash memory
- GPIO
- Peripherals
- Pins

### Learning Goal

Understand the difference between:
- Arduino board
- Arduino framework
- ATmega328P microcontroller

### Lab

Disassemble your mental model. Research:
- What chip is on your board?
- How much RAM?
- How much Flash?
- What peripherals exist?

Deliverable:
- A one-page "system diagram" of your Arduino.

## Unit 1: Digital I/O

### Concepts
- Voltage levels
- Inputs
- Outputs
-Pullups
- Floating inputs

### Learning Goal

Understand that digital inputs are physical voltages, not abstract booleans.

### Labs
#### Lab 1A

Blink LED.

#### Lab 1B

Control LED with button.

#### Lab 1C

Observe floating inputs. Remove pullups and watch erratic behavior.


## Unit 2: Time and Sequential Logic

### Concepts
- Program execution
- Blocking vs nonblocking code
- Timing

### Learning Goal

Understand why embedded software cannot spend all day waiting.

### Labs
#### Lab 2A

Blink with delay().

#### Lab 2B

Blink using millis().

#### Lab 2C

Two LEDs blinking independently.


## Unit 3: Finite State Machines

### Concepts
- State
- Events
- Transitions

### Learning Goal

Understand that embedded software is usually state-driven.

### Labs
#### Lab 3A

Button toggle.
States:
- ON
- OFF

#### Lab 3B

Traffic light controller.
States:
- GREEN
- YELLOW
- RED

#### Lab 3C

Three-mode LED controller. Button cycles modes.


## Unit 4: Interrupts
### Concepts
- Polling
- Interrupts
- ISR
- Concurrency

### Learning Goal

Understand why interrupts exist.

### Labs
#### Lab 4A

Button polling.

#### Lab 4B

Button interrupt.

#### Lab 4C

Compare behavior. Measure missed presses.


## Unit 5: Debouncing
### Concepts
- Mechanical switches
- Noise
- Event detection

### Learning Goal

Learn that real hardware is messy.

### Labs
#### Lab 5A

Observe bounce.

#### Lab 5B

Software debounce.

#### Lab 5C

Interrupt-based debounce.


## Unit 6: Analog Input
### Concepts
- ADC
- Sampling
- Quantization

### Learning Goal

Bridge physical signals and software.

### Labs
#### Lab 6A

Read potentiometer.

#### Lab 6B

Create software voltmeter.

#### Lab 6C

Measure noisy signal. Compute average.


## Unit 7: PWM
### Concepts
- Duty cycle
- Frequency
- Average voltage

### Learning Goal

Understand how digital hardware approximates analog behavior.

### Labs
#### Lab 7A

LED brightness control.

#### Lab 7B

Potentiometer controls brightness.

#### Lab 7C

Servo control.


## Unit 8A: Serial Communication (UART)

Simplest external communication.

### Concepts
- UART
- TX
- RX
- Baud rate

### Learning Goal

Understand communication between devices.

### Labs
#### Lab 8A

Print sensor readings.

#### Lab 8B

Accept commands.Example:
- LED ON
- LED OFF
- STATUS

#### Lab 8C

Interactive menu.

## Unit 8B: SPI Communications
### Concepts
- Synchronous serial communication
- Clocked data transfer
- MOSI/MISO/SCLK/SS
- Chip select vs addressing (contrast with I2C later)
- Master vs slave (light touch)

### Learning Goal
Understand how microcontrollers talk directly to other chips without an OS or negotiation layer.

### Labs
#### Lab 9B-A

SPI concept via Arduino-to-Arduino or loopback.
- Send bytes over SPI.
- Observe timing with scope/logic analyzer (or Serial print fallback).

#### Lab 9B-B

SPI device. Try one:
- SD card module (best real-world example)
- SPI OLED display
- External ADC (very educational)

#### Lab 9B-C

Compare UART vs SPI vs I2C. Same data sent, but different wiring and behavior.

## Unit 9: Structured Embedded Software
### Concepts
- Separation of concerns
- State machines
- Modules

### Labs

Refactor previous projects. Move from:
```
loop() {
    ...
}
```
to:
```
Button
LedController
StateMachine
Learning Goal
```
Write maintainable firmware.

## Unit 10: Timers
### Concepts
- Hardware timers
- Compare match
- Periodic events

### Learning Goal

Understand where timing really comes from.

### Labs
#### Lab 10A

Timer-based LED blinking.

#### Lab 10B

Timer interrupt.

#### Lab 10C

Software scheduler.


## Unit 11: I2C
### Concepts
- Bus architecture
- Master/slave
- Addressing

### Learning Goal

Connect external peripherals.

### Labs
#### Lab 11A

Read sensor.

#### Lab 11B

OLED display.

#### Lab 11C

Display live sensor data.


## Unit 12: Looking Under the Hood

The most important unit.

### Concepts
- Registers
- Memory-mapped peripherals
- Datasheets

### Learning Goal

Realize Arduino is just a layer on top of a microcontroller.

### Labs
#### Lab 12A

Read ATmega328P datasheet.Find:
- RAM size
- Flash size
- Timer count

#### Lab 12B

Replace Arduino function with direct register access. Example:
```
digitalWrite()
```
becomes direct manipulation of GPIO registers.

#### Lab 12C

Configure timer register manually.


## Final Project

Choose one:

### Digital thermostat

Uses:
- ADC
- State machine
- Display

### Reaction timer

Uses:
- Interrupts
- Timers
- UART

### Servo control system

Uses:
- PWM
- ADC
- State machine

### Custom project

Must demonstrate:
- State machine
- Interrupt
- Timer
- Communication interface


# Outcomes

After completing this course, you should be able to explain:

- What a microcontroller is
- Why interrupts exist
- How timers work
- What PWM is
- What ADC is
- How UART works
- Why debouncing is necessary
- How to structure embedded software
- What registers are
- How Arduino abstractions map to hardware

At that point, I'd consider you ready for Microcontrollers II, where you'd move to the ATmega328P itself (or STM32) and begin programming peripherals directly rather than primarily through the Arduino framework. That's roughly the point where Making Embedded Systems starts becoming a lot more relevant and enjoyable.