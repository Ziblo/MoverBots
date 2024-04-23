#ifndef CUSTOM_UUIDS_H
#define CUSTOM_UUIDS_H

#include <string>
#include <array>



#define SERVICE_UUID "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define NUM_OF_CHARACTERISTICS 3 //  <----  Don't forget to update this when adding a new one!

// Define an array of characteristic UUIDs
// https://www.uuidgenerator.net/ <---- use this to add more!
constexpr std::array<std::string, 3> characteristicUUIDs = {
    "beb5483e-36e1-4688-b7f5-ea07361b26a8",  // Characteristic UUID 0
    "f716693d-9321-4544-8952-5ba1ec9daad9",  // Characteristic UUID 1
    "ceef0256-6a0a-4850-a48e-619d79be55fc"   // Characteristic UUID 2
};

// Define an array of characteristic descriptions
constexpr std::array<std::string, 3> characteristicDescriptions = {
    "counting integer",                     // Description for Characteristic 0
    "Kim Ngo variable",                     // Description for Characteristic 1
    "Symbiosis number"                      // Description for Characteristic 2
};

#endif // CUSTOM_UUIDS_H
