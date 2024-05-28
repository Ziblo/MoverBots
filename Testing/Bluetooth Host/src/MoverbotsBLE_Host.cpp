#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "custom_UUIDs.h"

BLEService* InitService(BLEServer* pServer, const char* serv_uuid, unsigned int num_of_char, const customCharacteristic char_array[]){
    // Create the Master BLE Service
    BLEService *pService = pServer->createService(serv_uuid);
    
    //make pointers for the characteristics
    BLECharacteristic* pCharacteristics[num_of_char];

    // Create all the BLE Characteristics
    for (int i=0; i<num_of_char; i++){
        uint32_t properties = 0;
        switch (char_array[i].mode){
            case NOTIFY:
                properties = BLECharacteristic::PROPERTY_NOTIFY;
                break;
            case CALLBACK:
            case TWO_WAY: //Callback and Two-way both use read and write
                properties = BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE;
                break;
            default:
                //UNRECOGNISED MODE
                break;
        }
        pCharacteristics[i] = pService->createCharacteristic(char_array[i].UUID, properties);
    }

    // Create all the BLE Descriptors
    BLEDescriptor* pDescriptors[num_of_char]; //Characteristic User Descriptions
    for (int i=0; i<num_of_char; i++){
        pDescriptors[i] = new BLEDescriptor((uint16_t)0x2901); //descriptor for a Client Characteristic Configuration
        pDescriptors[i]->setValue(char_array[i].description);
        pCharacteristics[i]->addDescriptor(pDescriptors[i]);
        BLE2902* p2902 = new BLE2902(); //pointer to a generic descriptor for a Client Characteristic Configuration
        p2902->setNotifications(true);
        pCharacteristics[i]->addDescriptor(p2902);
    }
    *pCharacteristics_out = pCharacteristics;
    return pService;
}