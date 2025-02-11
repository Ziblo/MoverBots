#ifndef CUSTOM_UUIDS_H
#define CUSTOM_UUIDS_H

#include <string>
#include <array>

//Macros
#define MASTER_SERVICE_UUID "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"

//Enums
enum CharacteristicType {
    INTEGER,
    STRING,
    UWB_DATA,
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

//function prototypes
std::string BotServiceUUID(uint8_t index); //generate the bot service UUID based on an index from 0 to 255

//Master Characteristics
#define NUM_OF_MASTER_CHARACTERISTICS 2 //  <----  Don't forget to update this when adding a new one!
constexpr customCharacteristic MasterCharacteristics[] = {
//     UUID https://www.uuidgenerator.net/     DESCRIPTION             TYPE      MODE
    {"c96103e0-26e3-4d37-bf08-429f4a1aeba2", "Collective_Heading"   , INTEGER , NOTIFY},
    {"63a588af-acdc-4bd4-a579-574420f90435", "Num_of_Bots"          , INTEGER , NUM_OF_BOTS},
};

//Bot Specific Characteristics
#define NUM_OF_BOT_CHARACTERISTICS 4 //  <----  Don't forget to update this when adding a new one!
constexpr customCharacteristic BotCharacteristics[] = {
//     UUID https://www.uuidgenerator.net/     DESCRIPTION             TYPE      MODE
    {"beb5483e-36e1-4688-b7f5-ea07361b26a8", "Battery_Level"        , INTEGER , CALLBACK},
    {"f716693d-9321-4544-8952-5ba1ec9daad9", "UWB_Readings"         , UWB_DATA, PASSIVE},
    {"ceef0256-6a0a-4850-a48e-619d79be55fc", "dummy_data"           , STRING  , NOTIFY},
    {"8287d75f-a642-4fb9-9136-9e1f443eace9", "Read/Writeable"       , INTEGER , NOTIFY},
};

#endif // CUSTOM_UUIDS_H
