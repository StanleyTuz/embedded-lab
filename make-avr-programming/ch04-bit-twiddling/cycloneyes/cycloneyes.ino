// ----- Preamble ----- //
#include <avr/io.h>     // Defines pins, ports, etc.
#include <util/delay.h>

#define DELAYTIME   500 //85
#define LED_PORT    PORTB
#define LED_PIN     PINB
#define LED_DDR     DDRB

/* One LED sweeps across the row, back-and-forth. */
// int main(void) {
//     // ----- Initializations ----- //
//     uint8_t i = 0;
//     LED_DDR = 0xff; // set all pins in this port to output mode
    
//     while (1) {

//         while (i < 7) {
//             LED_PORT = (1 << i);    // turn on i-th pin
//             _delay_ms(DELAYTIME);
//             i++;
//         }

//         while (i > 0) {
//             LED_PORT = (1 << i);
//             _delay_ms(DELAYTIME);
//             i--;
//         }
//     }
//     return(0);
// }

/* Similar, but now all of them stay on, like a "bar graph". */
int main(void) {
    // ----- Initializations ----- //
    uint8_t i = 0;
    LED_DDR = 0xff; // set all pins in this port to output mode
    LED_PORT = 0;   // start with all off

    while (1) {
        while (i < 8) {
            LED_PORT |= (1 << i);    // turn on i-th pin
            _delay_ms(DELAYTIME);
            i++;
        }
        i--;

        while (i > 0) {
            LED_PORT &= ~(1 << i);
            _delay_ms(DELAYTIME);
            i--;
        }
    }
    return(0);
}