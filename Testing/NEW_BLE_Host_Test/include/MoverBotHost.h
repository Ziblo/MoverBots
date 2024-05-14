#ifndef MOVERBOTHOST_H
#define MOVERBOTHOST_H

#include <Arduino.h>
#include <list>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "custom_UUIDs.h"

class MoverBotHost {
public:
    MoverBotHost();
    ~MoverBotHost();

    BLEService* InitService(const char* serv_uuid, unsigned int num_of_char, const customCharacteristic char_array[]);

    bool is_connected();

    class MyServerCallbacks: public BLEServerCallbacks {
    public:
        MyServerCallbacks(MoverBotHost& host);
        void onConnect(BLEServer* pServer);
        void onDisconnect(BLEServer* pServer);

    private:
        MoverBotHost& host;
    };

private:
    BLEServer* pServer = nullptr;
    BLECharacteristic* pMasterCharacteristics[NUM_OF_MASTER_CHARACTERISTICS];
    std::list<BLECharacteristic*> botCharacteristicsList;
    bool deviceConnected = false;
};

#endif
