#ifndef CUSTOM_UUIDS_H
#define CUSTOM_UUIDS_H

#include <string>
#include <array>



#define SERVICE_UUID "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define NUM_OF_CHARACTERISTICS 3 //  <----  Don't forget to update this when adding a new one!

// Define an array of characteristic UUIDs
// https://www.uuidgenerator.net/ <---- use this to add more!
constexpr const char* characteristicUUIDs[] = {
    "beb5483e-36e1-4688-b7f5-ea07361b26a8",  // Characteristic UUID 0
    "f716693d-9321-4544-8952-5ba1ec9daad9",  // Characteristic UUID 1
    "ceef0256-6a0a-4850-a48e-619d79be55fc",  // Characteristic UUID 2
    "8287d75f-a642-4fb9-9136-9e1f443eace9",  // Characteristic UUID 3
};

// Define an array of characteristic descriptions
constexpr const char* characteristicDescriptions[] = {
    "counting by 1s",                     // Description for Characteristic 0
    "multiply by 2s",                     // Description for Characteristic 1
    "String test!!!",                     // Description for Characteristic 2
    "Read/Writeable",                     // Description for Characteristic 3
};

enum CharacteristicType {
    INTEGER,
    STRING
};

// Define an array of data types
constexpr CharacteristicType characteristicDataTypes[] = {
    INTEGER,  // Data type for Characteristic 0
    INTEGER,  // Data type for Characteristic 1
    STRING,   // Data type for Characteristic 2
    INTEGER,  // Data type for Characteristic 3
};

#endif // CUSTOM_UUIDS_H
