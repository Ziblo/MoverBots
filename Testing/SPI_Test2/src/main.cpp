#include <Arduino.h>
#include <SPI.h>

// Define which pins we're using
#define DWM1000_SCK_PIN 13
#define DWM1000_MISO_PIN 12
#define DWM1000_MOSI_PIN 11
#define DWM1000_SS_PIN 10
// #define DWM1000_SCK_PIN 14
// #define DWM1000_MISO_PIN 12
// #define DWM1000_MOSI_PIN 13
// #define DWM1000_SS_PIN 15

// Define SPI settings
#define BITORDER SPI_MSBFIRST
#define FREQUENCY 500000
#define DATA_MODE SPI_MODE3

//helper function prototypes
uint32_t DWM1000read(uint8_t header);
void DWM1000write(uint8_t header);

SPIClass MySPI;

void setup() {
  //initialize SPI pins
  MySPI.begin(DWM1000_SCK_PIN, DWM1000_MISO_PIN, DWM1000_MOSI_PIN, DWM1000_SS_PIN);
  //initialize SPI settings
  MySPI.setBitOrder(BITORDER);
  MySPI.setDataMode(DATA_MODE);
  MySPI.setFrequency(FREQUENCY);

  pinMode(MySPI.pinSS(),OUTPUT);

  //Initialize serial communication
  Serial.begin(115200);
}

void loop() {
  uint32_t result = DWM1000read(0x00); //read WHO_AM_I
  Serial.printf("result: 0x%x\n", result);
  delay(1000);
}

void DWM1000write(uint8_t register_id, uint32_t data){
  register_id = register_id & 0x3F;
  uint8_t header = register_id | 0x80;
  MySPI.beginTransaction(SPISettings(FREQUENCY, BITORDER, DATA_MODE));
  digitalWrite(MySPI.pinSS(), LOW);
  MySPI.transfer(header);
  for (int i=0; i<4; ++i) {
    MySPI.transfer((data >> (i * 8)) & 0xFF);
  }
  digitalWrite(MySPI.pinSS(), HIGH);
  MySPI.endTransaction();
}

uint32_t DWM1000read(uint8_t register_id){
  register_id = register_id & 0x3F;
  uint8_t header = register_id;
  MySPI.beginTransaction(SPISettings(FREQUENCY, BITORDER, DATA_MODE));
  digitalWrite(MySPI.pinSS(), LOW);
  MySPI.transfer(header);
  uint32_t dataIn = 0;
  for(int i=0; i<4; ++i){
    dataIn |= MySPI.transfer((0xAA)) << (i * 8);
  }
  digitalWrite(MySPI.pinSS(), HIGH);
  MySPI.endTransaction();
  return dataIn;
}