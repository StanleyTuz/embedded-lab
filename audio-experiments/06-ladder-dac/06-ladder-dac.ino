// ------- Preamble -------- //
#include <avr/io.h>

// Pin definitions
#define OUTPUT_PORT PORTA
#define OUTPUT_DDR DDRA

int main(void) {

  // -------- Inits --------- //
  // Set 4 pins to output mode
  OUTPUT_DDR = 0x0F; // P0-P3 are outputs

  // Set values
  // OUTPUT_PORT = 0b0001; // P0 is high
  // OUTPUT_PORT = 0b0011; // P0, P1 are high
  // OUTPUT_PORT = 0b0101; // P0, P2 are high
  // OUTPUT_PORT = 0b1000; // P3 is high
  OUTPUT_PORT = 0b1111; // all are high


  // ------ Event loop ------ //
  while (1) {
  }
  return 0;
}