/* This is just a driver for an 8-bit R-2R ladder DAC that I'm building. */
#include <avr/io.h>

// Pin definitions
#define OUTPUT_PORT PORTA
#define OUTPUT_DDR DDRA

int main(void) {

  // -------- Inits --------- //
  // Set 4 pins to output mode
  OUTPUT_DDR = 0xFF; // P0-P3 are outputs

  // Set values
  // OUTPUT_PORT = 0b0001; // P0 is high
  // OUTPUT_PORT = 0b11111111; // P0, P1 are high
  // OUTPUT_PORT = 0b0101; // P0, P2 are high
  // OUTPUT_PORT = 0b1000; // P3 is high
  OUTPUT_PORT = 0b00001111; // all are high


  // ------ Event loop ------ //
  while (1) {
  }
  return 0;
}