#include <SPI.h>
#include <Arduino.h>

// Function prototypes
void configureUWBCalibration();// Declare configureUWBCalibration set up function
uint8_t readRegister(uint8_t regAddr); // Declare the readRegister function

// Define DWM1000 SPI pins
#define DWM1000_SCK_PIN 13
#define DWM1000_MISO_PIN 12
#define DWM1000_MOSI_PIN 11
#define DWM1000_SS_PIN 10

// Define DWM1000 register addresses
#define DWM1000_DEV_ID_REG 0x00 // Device ID register address
#define DWM1000_SYS_CFG_REG 0x0D // System Configuration register address
#define DWM1000_TC_PGDELAY_REG 0x2A // TX Power and RX Time Delay register address

void setup() {
    Serial.begin(115200); // Initialize serial communication
    
    // Initialize SPI communication
    SPI.begin();
    SPI.beginTransaction(SPISettings(3000000, MSBFIRST, SPI_MODE0)); // Set SPI frequency to 3 MHz and mode 0
    
    // Initialize DWM1000 chip select pin
    pinMode(DWM1000_SS_PIN, OUTPUT);
    digitalWrite(DWM1000_SS_PIN, HIGH); // Deselect DWM1000 initially
    
    // Configure DWM1000 for UWB calibration
    configureUWBCalibration();
}

void loop() {
    // Read device ID register from DWM1000
    uint8_t deviceID = readRegister(DWM1000_DEV_ID_REG);
    
    // Print device ID
    Serial.print("DWM1000 Device ID: 0x");
    Serial.println(deviceID, HEX);
    
    // Wait before reading again
    delay(1000);
}

// Function to read a register from the DWM1000
uint8_t readRegister(uint8_t regAddr) {
    digitalWrite(DWM1000_SS_PIN, LOW); // Select DWM1000
    
    // Send read command (bit 7 set)
    SPI.transfer(regAddr | 0x80); // OR with 0x80 for read
    
    // Read data from register
    uint8_t data = SPI.transfer(0x00); // Send dummy data to read
    
    digitalWrite(DWM1000_SS_PIN, HIGH); // Deselect DWM1000
    
    return data;
}

// Function to write a value to a register in the DWM1000
void writeRegister(uint8_t regAddr, uint8_t value) {
    digitalWrite(DWM1000_SS_PIN, LOW); // Select DWM1000
    
    // Send write command (bit 7 clear)
    SPI.transfer(regAddr & 0x7F); // AND with 0x7F for write
    
    // Send data to write to register
    SPI.transfer(value);
    
    digitalWrite(DWM1000_SS_PIN, HIGH); // Deselect DWM1000
}

// Function to configure DWM1000 for UWB calibration
void configureUWBCalibration() {
    // Configure system configuration register (enable UWB calibration)
    writeRegister(DWM1000_SYS_CFG_REG, 0x01); // Set bit 0 to enable UWB calibration
    
    // Configure TX power and RX time delay register for calibration (example values)
    writeRegister(DWM1000_TC_PGDELAY_REG, 0x0C); // Set TX power and RX time delay (example)
}
