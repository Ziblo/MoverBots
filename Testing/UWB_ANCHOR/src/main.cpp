#include <Arduino.h>
#include <SPI.h>
#include <DW1000.h>
#include "DW1000Ranging.h"


// Define which pins we're using
#define DWM1000_SCK_PIN 13
#define DWM1000_MISO_PIN 12
#define DWM1000_MOSI_PIN 11
#define DWM1000_SS_PIN 10
#define RST_PIN 4
#define IRQ_PIN 5

// Define SPI settings
#define BITORDER SPI_MSBFIRST
#define FREQUENCY 1000000
#define DATA_MODE SPI_MODE2

//helper function prototypes
uint32_t DWM1000read(uint8_t header);
void DWM1000write(uint8_t header);
void newRange();
void newBlink(DW1000Device* device);
void inactiveDevice(DW1000Device* device);

SPIClass MySPI;
DW1000Class myDW1000;
DW1000RangingClass myRanging;


void setup() {
  Serial.begin(115200);
  delay(1000);
  //initialize SPI pins
  DW1000.begin(IRQ_PIN, RST_PIN);
  //MySPI.begin(DWM1000_SCK_PIN, DWM1000_MISO_PIN, DWM1000_MOSI_PIN, DWM1000_SS_PIN);
  
  //initialize SPI settings
  MySPI.setBitOrder(BITORDER);
  MySPI.setDataMode(DATA_MODE);
  MySPI.setFrequency(FREQUENCY);
  

  pinMode(MySPI.pinSS(), OUTPUT);
  
  //DW1000 Config
  myRanging.initCommunication(RST_PIN, DWM1000_SS_PIN, IRQ_PIN);
  myRanging.attachNewRange(newRange);
  myRanging.attachBlinkDevice(newBlink);
  myRanging.attachInactiveDevice(inactiveDevice);
  char anchorAddress[] = "83:17:5B:D5:A9:9A:E2:9C";
  DW1000Ranging.startAsAnchor(anchorAddress, DW1000.MODE_LONGDATA_RANGE_ACCURACY, false);
}

void loop() {
    myRanging.loop();
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

void newRange() {
  Serial.print("from: "); Serial.print(DW1000Ranging.getDistantDevice()->getShortAddress(), HEX);
  Serial.print("\t Range: "); Serial.print(DW1000Ranging.getDistantDevice()->getRange()); Serial.print(" m");
  Serial.print("\t RX power: "); Serial.print(DW1000Ranging.getDistantDevice()->getRXPower()); Serial.println(" dBm");
}

void newBlink(DW1000Device* device) {
  Serial.print("blink; 1 device added ! -> ");
  Serial.print(" short:");
  Serial.println(device->getShortAddress(), HEX);
}

void inactiveDevice(DW1000Device* device) {
  Serial.print("delete inactive device: ");
  Serial.println(device->getShortAddress(), HEX);
}

