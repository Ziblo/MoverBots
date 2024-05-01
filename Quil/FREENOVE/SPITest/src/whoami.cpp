#include <Arduino.h>
#include "SPI.h"

// Define SPI pins
#define SCK_PIN 12  // GPIO pin for SCK
#define MISO_PIN 11 // GPIO pin for MISO
#define MOSI_PIN 13 // GPIO pin for MOSI
#define SS_PIN 10    // GPIO pin for SS

void setup() {
    // Initialize SPI communication
    SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);

    // Set SPI parameters
    SPI.setDataMode(SPI_MODE0);     // Set data mode (0, 1, 2, or 3)
    SPI.setBitOrder(MSBFIRST);       // Set bit order (MSBFIRST or LSBFIRST)
    SPI.setClockDivider(SPI_CLOCK_DIV4); // Set clock divider (1, 2, 4, 8, 16, 32, 64, or 128)
    SPI.setFrequency(1000000);    // Optionally set SPI frequency in Hz (default is 1MHz)

    // Initialize Serial communication for debugging
    Serial.begin(115200);
    while (!Serial); // Wait for Serial to initialize
}

void loop() {
    // Send request to SPI device
    digitalWrite(SS_PIN, LOW); // Select the SPI device
    SPI.transfer(0x01); // Send a request command (assuming 0x01 is the command to request identification)
    byte response = SPI.transfer(0x00); // Read the response from the SPI device
    digitalWrite(SS_PIN, HIGH); // Deselect the SPI device

    // Print response
    Serial.print("SPI device ID: ");
    Serial.println(response, HEX);

    // Wait for a while before sending the next request
    delay(1000);
}