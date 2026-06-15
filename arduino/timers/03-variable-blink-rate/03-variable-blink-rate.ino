// /* Version 1: using millis() --- this is polling.

// Allow an external potentiometer to control the blink rate of an LED.
// The LED should blink at a rate between [100ms, 1000ms].
// */

// unsigned long blink_rate_ms;
// unsigned long last_blink_ms;
// unsigned long now;
// bool led_on;

// unsigned long calc_blink_rate(int pot_value) {
//     return (int)((100.0 + 900.0 * (pot_value / 1023.0))/2);
// }

// void setup() {
//     // Serial.begin(9600);
//     pinMode(13, OUTPUT);
//     pinMode(A0, INPUT);

//     blink_rate_ms = calc_blink_rate(analogRead(A0));
//     last_blink_ms = millis();
//     led_on = false;
// }

// void loop() {
//     // Read the potentiometer
//     int pot_value = analogRead(A0); // 0 - 1023
//     // Update the blink rate
//     blink_rate_ms = calc_blink_rate(pot_value);

//     // Serial.println(blink_rate_ms);

//     now = millis();
//     if (now - last_blink_ms >= blink_rate_ms) {
//         last_blink_ms += blink_rate_ms;
//         led_on = !led_on;
//         digitalWrite(13, (led_on) ? HIGH : LOW);
//     }
    
// }


/* Version 2: using a timer interrupt.

Allow an external potentiometer to control the blink rate of an LED.
The LED should blink at a rate between [100ms, 1000ms].
*/
unsigned long loop_count;
unsigned long blink_rate_ms;
unsigned long last_blink_ms;
unsigned long now;
bool led_on;

unsigned long calc_blink_rate_ms(int pot_value) {
    return (int)((100.0 + 900.0 * (pot_value / 1023.0))/2);
}

void setup_timer1() {
    /* Configure Timer1 interrupt. */
    cli();

    // Clear Timer/Counter 1 Control Registers
    TCCR1A = 0;
    TCCR1B = 0;

    // Reset counter
    TCNT1 = 0;

    // Set mode to CTC (Clear Timer on Compare Match)
    TCCR1B |= (1 << WGM12);

    // Set the prescaler to 256
    TCCR1B |= (1 << CS12);

    // Enable the interrupt on Compare Match A
    TIMSK1 |= (1 << OCIE1A);

    sei();
}

void calc_set_timer_ocr(unsigned long blink_rate_ms) {
    /* Given a blink rate in ms, set the Output Compare Register accordingly. */
    cli();
    OCR1A = blink_rate_ms / 0.016 - 1;
    sei();
}

void setup() {
    pinMode(13, OUTPUT);
    pinMode(A0, INPUT);

    blink_rate_ms = calc_blink_rate_ms(analogRead(A0));
    calc_set_timer_ocr(blink_rate_ms);

    loop_count = 0;

    setup_timer1();
    led_on = false;
}

void loop() {
    if (loop_count >= 500) {
        loop_count = 0;
        // Read the potentiometer
        blink_rate_ms = calc_blink_rate_ms(analogRead(A0));
        // Update the blink rate
        calc_set_timer_ocr(blink_rate_ms);
    }

    // Serial.println(blink_rate_ms);
    loop_count++;
}

ISR(TIMER1_COMPA_vect) {
    // Toggle the LED when this timer interrupt fires
    PINB = (1 << PB7);
}