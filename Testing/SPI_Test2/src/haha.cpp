// #include <SPI.h>
// #include <DW1000Ranging.h>
 
// // #define DWM1000_SCK_PIN 13
// // #define DWM1000_MISO_PIN 12
// // #define DWM1000_MOSI_PIN 11
// // #define DWM1000_SS_PIN 10
// #define SPI_SCK 13
// #define SPI_MISO 12
// #define SPI_MOSI 11
// #define DW_CS 10
 
// // connection pins
// const uint8_t PIN_RST = 4; // reset pin
// const uint8_t PIN_IRQ = 5; // irq pin
// const uint8_t PIN_SS = 10;   // spi select pin

// //lehelper
// void newRange();
// void newDevice(DW1000Device *device);
// void inactiveDevice(DW1000Device *device);

 
// void setup()
// {
//     Serial.begin(115200);
//     delay(1000);
//     //init the configuration
//     SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI, PIN_SS);
//     SPI.setBitOrder(SPI_MSBFIRST);
//     SPI.setDataMode(SPI_MODE2);
//     SPI.setFrequency(1000000);
//     DW1000Ranging.initCommunication(PIN_RST, PIN_SS, PIN_IRQ); //Reset, CS, IRQ pin
//     //define the sketch as anchor. It will be great to dynamically change the type of module
//     DW1000Ranging.attachNewRange(newRange);
//     DW1000Ranging.attachNewDevice(newDevice);
//     DW1000Ranging.attachInactiveDevice(inactiveDevice);
//     //Enable the filter to smooth the distance
//     //DW1000Ranging.useRangeFilter(true);
 
//     //we start the module as a tag
//     char tagAddress[] = "7D:00:22:EA:82:60:3B:9C";
//     DW1000Ranging.startAsTag(tagAddress, DW1000.MODE_LONGDATA_RANGE_LOWPOWER);
// }
 
// void loop()
// {
//     DW1000Ranging.loop();
// }
 
// void newRange()
// {
//     Serial.print("from: ");
//     Serial.print(DW1000Ranging.getDistantDevice()->getShortAddress(), HEX);
//     Serial.print("\t Range: ");
//     Serial.print(DW1000Ranging.getDistantDevice()->getRange());
//     Serial.print(" m");
//     Serial.print("\t RX power: ");
//     Serial.print(DW1000Ranging.getDistantDevice()->getRXPower());
//     Serial.println(" dBm");
// }
 
// void newDevice(DW1000Device *device)
// {
//     Serial.print("ranging init; 1 device added ! -> ");
//     Serial.print(" short:");
//     Serial.println(device->getShortAddress(), HEX);
// }
 
// void inactiveDevice(DW1000Device *device)
// {
//     Serial.print("delete inactive device: ");
//     Serial.println(device->getShortAddress(), HEX);
// }