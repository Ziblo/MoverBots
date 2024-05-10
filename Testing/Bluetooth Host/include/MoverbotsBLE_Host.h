#ifndef MOVERBOTSBLE_HOST_H
#define MOVERBOTSBLE_HOST_H

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "custom_UUIDs.h"

BLEService* InitService(BLEServer* pServer, const char* serv_uuid, unsigned int num_of_char, const customCharacteristic char_array[]);

#endif // MOVERBOTSBLE_HOST_H
