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

    BLEService* InitService(BLEUUID serv_uuid, unsigned int num_of_char, const customCharacteristic char_array[]);

    bool is_connected();
    unsigned int num_of_bots;

    class MyServerCallbacks: public BLEServerCallbacks {
    public:
        MyServerCallbacks(MoverBotHost& host);
        void onConnect(BLEServer* pServer);
        void onDisconnect(BLEServer* pServer);

    private:
        MoverBotHost& host;
    };

    class CharCallback_Flag_On_Write : public BLECharacteristicCallbacks {
    public:
        void onWrite(BLECharacteristic *pChar) override;
    };
    class CharCallback_Num_Of_Bots : public BLECharacteristicCallbacks {
    public:
        CharCallback_Num_Of_Bots(MoverBotHost& host);
        void onWrite(BLECharacteristic *pChar) override;
    private:
        MoverBotHost& host;
        unsigned int num_of_bot_services_created = 0;
    };

private:
    BLEServer* pServer = nullptr;
    BLECharacteristic* pMasterCharacteristics[NUM_OF_MASTER_CHARACTERISTICS];
    std::list<BLECharacteristic*> botCharacteristicsList;
    bool deviceConnected = false;
};

#endif
