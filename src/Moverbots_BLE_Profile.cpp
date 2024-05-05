
#include <string>
#include "Moverbots_BLE_Profile.h"

//function implementations

//generate the bot service UUID based on an index from 0 to 255
std::string BotServiceUUID(uint8_t index) {    
    return "674cd48d-ea7a-4621-b2df-e8f71ad559" + std::to_string(index / 16) + std::to_string(index % 16);
}
