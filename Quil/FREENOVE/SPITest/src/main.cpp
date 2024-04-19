#include <Arduino.h> // Include the Arduino core library
#include "SPI.h"

// Define SPI pins
#define SCK_PIN 18  // GPIO pin for SCK
#define MISO_PIN 19 // GPIO pin for MISO
#define MOSI_PIN 23 // GPIO pin for MOSI
#define SS_PIN 5    // GPIO pin for SS

void setup() {
    // Initialize SPI communication
    SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);

    // Set SPI parameters
    SPI.setDataMode(SPI_MODE0);     // Set data mode (0, 1, 2, or 3)
    SPI.setBitOrder(MSBFIRST);       // Set bit order (MSBFIRST or LSBFIRST)
    SPI.setClockDivider(SPI_CLOCK_DIV4); // Set clock divider (1, 2, 4, 8, 16, 32, 64, or 128)
    // SPI.setFrequency(1000000);    // Optionally set SPI frequency in Hz (default is 1MHz)
}

void loop() {
    // Your main program logic here
}