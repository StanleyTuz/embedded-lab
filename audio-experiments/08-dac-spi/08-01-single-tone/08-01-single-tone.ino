
#include <Arduino.h>
#include <SPI.h>

#define SPI_PORT    PORTB
#define NSS_PIN     53  // PB0
#define SCK_PIN     52  // PB1
#define MOSI_PIN    51  // PB2
#define MISO_PIN    50  // PB3

void setup(){

    // Start the SPI
    SPI.begin();

    // Set modes of the SPI pins
    pinMode(NSS_PIN, OUTPUT);
    pinMode(SCK_PIN, OUTPUT);
    pinMode(MOSI_PIN, OUTPUT);
    pinMode(MISO_PIN, INPUT);
    
    // Configure SPI to work with the MCP4921
    SPCR &= ~(1 << 5);  // DORD = MSB
    SPCR |= (1 << 4);   // MSTR = 1 (master)
    SPCR &= ~(1 << 3);  // CPOL = 0
    SPCR &= ~(1 << 2);  // CPHA = 0
    SPCR |= (1 << 1);   // clock = fosc / 64

}

uint16_t get_write_command(uint16_t value){
    uint16_t writeCmd = value;

    // unset bit 15
    writeCmd &= ~(1 << 15);
    // buffer control bit
    writeCmd &= ~(1 << 14);
    // output gain selection
    writeCmd |= (1 << 13);  // x1
    // output shutdown control
    writeCmd |= (1 << 12);  // Vout active

    return writeCmd;
}

void loop(){
    // arbitrary 12-bit number
    
    uint16_t value = 0x400;
    
    uint16_t writeCmd = get_write_command(value);
    
    digitalWrite(NSS_PIN, LOW);
    SPI.transfer16(writeCmd);
    digitalWrite(NSS_PIN, HIGH);

    // delayMicroseconds(50);

    while(1);
}