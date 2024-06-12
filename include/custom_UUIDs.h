#ifndef CUSTOM_UUIDS_H
#define CUSTOM_UUIDS_H

#include <string>
#include <array>

//Macros
#define SERVICE_UUID "8845e8ec-6a6b-43f9-a08b-a30d7c0f9d22"

//Enums
enum CharacteristicType {
    INTEGER,
    STRING,
    UWB_DATA,
    GAMEPAD_EVENT,
};
enum CharacteristicMode {
    NOTIFY,     //Host sends updates to the client
    CALLBACK,   //Client send updates to the host
    PASSIVE,    //they both can update a variable without notifying either
    TWO_WAY,    //they can both write which notifies the other
    H_REQUEST,  //Host requests this data from the client
    C_REQUEST,  //Client requests this data from the host
    NUM_OF_BOTS,//send a signal to the host to create/close a new bot service when this characteristic changes
};

struct customCharacteristic {
    const char* UUID;
    const char* description;
    CharacteristicType type;
    CharacteristicMode mode;
};

//Bot Specific Characteristics
#define NUM_OF_CHARACTERISTICS 2 //  <----  Don't forget to update this when adding a new one!
constexpr customCharacteristic MoverBotCharacteristics[] = {
//     UUID https://www.uuidgenerator.net/     DESCRIPTION                TYPE      MODE
    {"e3a47b8f-171f-4761-8032-731f4395eb06", "Gamepad_Input_ID"        , INTEGER , NOTIFY},
    {"f716693d-9321-4544-8952-5ba1ec9daad9", "Gamepad_Input_Data"      , INTEGER , NOTIFY},
    // {"ceef0256-6a0a-4850-a48e-619d79be55fc", "dummy_data"           , INTEGER , NOTIFY},
    // {"8287d75f-a642-4fb9-9136-9e1f443eace9", "Read/Writeable"       , INTEGER , NOTIFY},
};

#endif // CUSTOM_UUIDS_H
