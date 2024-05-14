
#include <string>
#include "custom_UUIDs.h"
#include <BLECharacteristic.h>
//function implementations

//generate the bot service UUID based on an index from 0 to 255
std::string BotServiceUUID(uint8_t index) {    
    return "674cd48d-ea7a-4621-b2df-e8f71ad559" + std::to_string(index / 16) + std::to_string(index % 16);
}

BLECharacteristicCallbacks* getCallback(CallbackType type) {
    //allocate a new callback so it won't get garbage collected
    switch (type) {
        case CUSTOM1: return new MyCustomCharacteristicCallback1();
        case CUSTOM2: return new MyCustomCharacteristicCallback2();
        case NO_CALLBACK:
        default: return nullptr;
    }
}

//Make each callback type
class MyCustomCharacteristicCallback1: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pChar) override {
        //do something on write
        Serial.print("Custom 1 just saw this one: ");
        Serial.println(pChar->getValue().c_str());
    }
};
class MyCustomCharacteristicCallback2: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pChar) override {
        //do something on write
        Serial.println("Custom 2 is here for you!");
    }
};