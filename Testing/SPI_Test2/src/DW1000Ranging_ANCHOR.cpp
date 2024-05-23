// // // /**
// // //  * 
// // //  * @todo
// // //  *  - move strings to flash (less RAM consumption)
// // //  *  - fix deprecated convertation form string to char* startAsAnchor
// // //  *  - give example description
// // //  */
// // // #include <SPI.h>
// // // #include "DW1000Ranging.h"

// // // // connection pins
// // // const uint8_t PIN_RST = 9; // reset pin
// // // const uint8_t PIN_IRQ = 2; // irq pin
// // // const uint8_t PIN_SS = SS; // spi select pin

// // // void setup() {
// // //   Serial.begin(115200);
// // //   delay(1000);
// // //   //init the configuration
// // //   DW1000Ranging.initCommunication(PIN_RST, PIN_SS, PIN_IRQ); //Reset, CS, IRQ pin
// // //   //define the sketch as anchor. It will be great to dynamically change the type of module
// // //   DW1000Ranging.attachNewRange(newRange);
// // //   DW1000Ranging.attachBlinkDevice(newBlink);
// // //   DW1000Ranging.attachInactiveDevice(inactiveDevice);
// // //   //Enable the filter to smooth the distance
// // //   //DW1000Ranging.useRangeFilter(true);
  
// // //   //we start the module as an anchor
// // //   DW1000Ranging.startAsAnchor("82:17:5B:D5:A9:9A:E2:9C", DW1000.MODE_LONGDATA_RANGE_ACCURACY);
// // // }

// // // void loop() {
// // //   DW1000Ranging.loop();
// // // }



// // // void newRange() {
// // //   Serial.print("from: "); Serial.print(DW1000Ranging.getDistantDevice()->getShortAddress(), HEX);
// // //   Serial.print("\t Range: "); Serial.print(DW1000Ranging.getDistantDevice()->getRange()); Serial.print(" m");
// // //   Serial.print("\t RX power: "); Serial.print(DW1000Ranging.getDistantDevice()->getRXPower()); Serial.println(" dBm");
// // // }

// // // void newBlink(DW1000Device* device) {
// // //   Serial.print("blink; 1 device added ! -> ");
// // //   Serial.print(" short:");
// // //   Serial.println(device->getShortAddress(), HEX);
// // // }

// // // void inactiveDevice(DW1000Device* device) {
// // //   Serial.print("delete inactive device: ");
// // //   Serial.println(device->getShortAddress(), HEX);
// // // }

// /**
//  * This example demonstrates how to set up an anchor node using the DW1000 UWB module with Arduino.
//  * 
//  * @todo
//  *  - Move strings to flash (less RAM consumption)
//  *  - Fix deprecated conversion from string to char* in startAsAnchor
//  *  - Provide example description
//  */

// /**
//  * This example demonstrates how to set up an anchor node using the DW1000 UWB module with Arduino.
//  * 
//  * @todo
//  *  - Move strings to flash (less RAM consumption)
//  *  - Fix deprecated conversion from string to char* in startAsAnchor
//  *  - Provide example description
//  */

// #include <SPI.h>
// #include "DW1000Ranging.h"

// // Connection pins
// const uint8_t PIN_RST = 4; // Reset pin
// const uint8_t PIN_IRQ = 5; // IRQ pin
// const uint8_t PIN_SS = SS; // SPI select pin
// uint8_t spiGetDataMode(spi_t * spi);

// void newRange();
// void newBlink(DW1000Device* device);
// void inactiveDevice(DW1000Device* device);

// void setup() {
//   Serial.begin(115200);
//   delay(1000);

//   // Initialize the DW1000 communication
//   DW1000Ranging.initCommunication(PIN_RST, PIN_SS, PIN_IRQ); // Reset, CS, IRQ pin
  

//   // Attach callbacks
//   DW1000Ranging.attachNewRange(newRange);
//   DW1000Ranging.attachBlinkDevice(newBlink);
//   DW1000Ranging.attachInactiveDevice(inactiveDevice);

//   // Enable the filter to smooth the distance measurements (optional)
//   // DW1000Ranging.useRangeFilter(true);
  
//   // Start the module as an anchor
//   char anchorAddress[] = "82:17:5B:D5:A9:9A:E2:9C";
//   DW1000Ranging.startAsAnchor(anchorAddress, DW1000.MODE_LONGDATA_RANGE_ACCURACY);
// }

// void loop() {
//   DW1000Ranging.loop();
// }

// void newRange() {
//   Serial.print(F("from: ")); Serial.print(DW1000Ranging.getDistantDevice()->getShortAddress(), HEX);
//   Serial.print(F("\t Range: ")); Serial.print(DW1000Ranging.getDistantDevice()->getRange()); Serial.print(F(" m"));
//   Serial.print(F("\t RX power: ")); Serial.print(DW1000Ranging.getDistantDevice()->getRXPower()); Serial.println(F(" dBm"));
// }

// void newBlink(DW1000Device* device) {
//   Serial.print(F("blink; 1 device added! -> "));
//   Serial.print(F(" short:"));
//   Serial.println(device->getShortAddress(), HEX);
// }

// void inactiveDevice(DW1000Device* device) {
//   Serial.print(F("delete inactive device: "));
//   Serial.println(device->getShortAddress(), HEX);
// }
