#ifndef CUSTOM_UUIDS_H
#define CUSTOM_UUIDS_H

#include <string>
#include <array>

enum CharacteristicType {
    INTEGER,
    STRING,
};

enum CharacteristicMode {
    NOTIFY,     //Host sends updates to the client
    CALLBACK,   //Client send updates to the host
    POLLING,    //they both can update a variable without notifying either
    TWO_WAY,    //they can both write which notifies the other
    H_REQUEST,  //Host requests this data from the client
    C_REQUEST,  //Client requests this data from the host  
};

struct customCharacteristic {
    const char* UUID;
    const char* description;
    CharacteristicType type;
    CharacteristicMode mode;
};

#define SERVICE_UUID "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define NUM_OF_CHARACTERISTICS 4 //  <----  Don't forget to update this when adding a new one!

constexpr customCharacteristic customCharacteristics[] = {
//     UUID https://www.uuidgenerator.net/     DESCRIPTION         TYPE    MODE
    {"beb5483e-36e1-4688-b7f5-ea07361b26a8", "counting by 1s"   , INTEGER, NOTIFY},
    {"f716693d-9321-4544-8952-5ba1ec9daad9", "multiply by 2s"   , INTEGER, NOTIFY},
    {"ceef0256-6a0a-4850-a48e-619d79be55fc", "String test!!!"   , STRING , NOTIFY},
    {"8287d75f-a642-4fb9-9136-9e1f443eace9", "Read/Writeable"   , INTEGER, NOTIFY},
};

#endif // CUSTOM_UUIDS_H
