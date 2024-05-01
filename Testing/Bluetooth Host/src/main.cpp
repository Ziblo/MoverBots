/*
    Video: https://www.youtube.com/watch?v=oCMOYS71NIU
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updated by chegewara

   Create a BLE server that, once we receive a connection, will send periodic notifications.
   The service advertises itself as: 4fafc201-1fb5-459e-8fcc-c5c9c331914b
   And has a characteristic of: beb5483e-36e1-4688-b7f5-ea07361b26a8

   The design of creating the BLE server is:
   1. Create a BLE Server
   2. Create a BLE Service
   3. Create a BLE Characteristic on the Service
   4. Create a BLE Descriptor on the characteristic
   5. Start the service.
   6. Start advertising.

   A connect hander associated with the server starts a background task that performs notification
   every couple of seconds.
*/
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "custom_UUIDs.h"

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristics[NUM_OF_CHARACTERISTICS];
bool deviceConnected = false;
bool oldDeviceConnected = false;


class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      BLEDevice::startAdvertising();
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};



void setup() {
  Serial.begin(115200);

  // Create the BLE Device
  BLEDevice::init("ESP32 Host");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create all the BLE Characteristics
  for (int i=0; i<NUM_OF_CHARACTERISTICS; i++){
    pCharacteristics[i] = pService->createCharacteristic(
                      customCharacteristics[i].UUID,
                      BLECharacteristic::PROPERTY_NOTIFY);
  }

  // Create all the BLE Descriptors
  BLEDescriptor* pDescriptors[NUM_OF_CHARACTERISTICS]; //Characteristic User Descriptions
  for (int i=0; i<NUM_OF_CHARACTERISTICS; i++){
    pDescriptors[i] = new BLEDescriptor((uint16_t)0x2901); //descriptor for a Client Characteristic Configuration
    pDescriptors[i]->setValue(customCharacteristics[i].description);
    pCharacteristics[i]->addDescriptor(pDescriptors[i]);
    BLE2902* p2902 = new BLE2902(); //pointer to a generic descriptor for a Client Characteristic Configuration
    p2902->setNotifications(true);
    pCharacteristics[i]->addDescriptor(p2902);
  }

  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true); //needs this to be true to show service uuid in advertisement
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
  Serial.println("Waiting a client connection to notify...");
}

void loop() {
    // notify changed value
    static uint32_t value0 = 0;
    static uint32_t value1 = 1;
    std::string valueString; //I haven't had any issues sending 77 characters over BLE
    if (deviceConnected) {
        //update the characteristics

        valueString = "num0: " + std::to_string(value0) + ", num1: " + std::to_string(value1) + ". The sly brown fox jumped over the lazy dog. 0123456789";

        pCharacteristics[0]->setValue(value0);
        pCharacteristics[0]->notify();
        Serial.print("sending in characteristic 0: ");
        Serial.println(value0);

        pCharacteristics[1]->setValue(value1);
        pCharacteristics[1]->notify();
        Serial.print("sending in characteristic 1: ");
        Serial.println(value1);

        pCharacteristics[2]->setValue(valueString);
        pCharacteristics[2]->notify();
        Serial.print("sending in characteristic 2: ");
        Serial.println(valueString.c_str());

        value0 += 1; //count by 1s
        value1 *= 2; //multiply by 2s
        if (!value1) value1 = 1;
        
        delay(250); // bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
    }
    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
}
