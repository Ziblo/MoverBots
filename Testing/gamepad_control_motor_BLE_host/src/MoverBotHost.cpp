// Include any necessary headers
#include "MoverBotHost.h"

//Constructor Definitiion
MoverBotHost::MoverBotHost(){
    // Create the BLE Device
    BLEDevice::init("MoverBot Host");
    // Create the BLE Server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks(*this)); //pass in reference to self
    //create the master service!
    BLEService* pMoverBotService = InitService((std::string) SERVICE_UUID, NUM_OF_CHARACTERISTICS, MoverBotCharacteristics);
    //Start the Master Service
    pMoverBotService->start();

    // Start advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true); //needs this to be true to show service uuid in advertisement
    pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
    BLEDevice::startAdvertising();
}

//Destructor Definition?
MoverBotHost::~MoverBotHost(){
    //clean up resources if needed
}

//Initialize a BLE service
BLEService* MoverBotHost::InitService(BLEUUID serv_uuid, unsigned int num_of_char, const customCharacteristic char_array[]){
    // Create the Master BLE Service
    BLEService *pService = pServer->createService(serv_uuid);
    
    //make pointers for the characteristics
    BLECharacteristic* pCharacteristics[num_of_char];

    // Create all the BLE Characteristics
    for (int i=0; i<num_of_char; i++){
        uint32_t properties = 0;
        switch (char_array[i].mode){
            case NOTIFY:
                properties = BLECharacteristic::PROPERTY_NOTIFY;
                break;
            case CALLBACK:
            case PASSIVE:
            case NUM_OF_BOTS:
            case TWO_WAY: //Callback and Two-way both use read and write
                properties = BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE;
                break;
            default:
                //UNRECOGNISED MODE
                break;
        }
        pCharacteristics[i] = pService->createCharacteristic(char_array[i].UUID, properties);
    }

    // Create all the BLE Descriptors
    BLEDescriptor* pDescriptors[num_of_char]; //Characteristic User Descriptions
    for (int i=0; i<num_of_char; i++){
        pDescriptors[i] = new BLEDescriptor((uint16_t)0x2901); //descriptor for a Client Characteristic Configuration
        pDescriptors[i]->setValue(char_array[i].description);
        pCharacteristics[i]->addDescriptor(pDescriptors[i]);
        BLE2902* p2902 = new BLE2902(); //pointer to a generic descriptor for a Client Characteristic Configuration
        p2902->setNotifications(true);
        pCharacteristics[i]->addDescriptor(p2902);
    }

    // Now can add the callback function here if needed
    for (int i=0; i<num_of_char; i++){
        switch (char_array[i].mode){
            case CALLBACK:
                pCharacteristics[i]->setCallbacks(new CharCallback_Flag_On_Write());
                break;
            case NUM_OF_BOTS:
                pCharacteristics[i]->setCallbacks(new CharCallback_Num_Of_Bots(*this));
                break;
        }
    }
    return pService;
}

//make the custom server callback functions (based on BLEServerCallbacks)
MoverBotHost::MyServerCallbacks::MyServerCallbacks(MoverBotHost& _host) : host(_host) {} //automatically save _host as host

//Callback when a device connects
void MoverBotHost::MyServerCallbacks::onConnect(BLEServer* pServer) {
    host.deviceConnected = true;
    BLEDevice::startAdvertising();
}
//Callback when a device disconnects
void MoverBotHost::MyServerCallbacks::onDisconnect(BLEServer* pServer) {
    host.deviceConnected = false;
}
//Callback when a characteristic (in CALLBACK mode) is written to
void MoverBotHost::CharCallback_Flag_On_Write::onWrite(BLECharacteristic *pChar) {
    //do something
}
//Callback when a num_of_bots changes?
MoverBotHost::CharCallback_Num_Of_Bots::CharCallback_Num_Of_Bots(MoverBotHost& _host) : host(_host) {}
void MoverBotHost::CharCallback_Num_Of_Bots::onWrite(BLECharacteristic *pChar) {
    //do something with host (host.is_connected() etc)
}

bool MoverBotHost::is_connected(){
    return deviceConnected;
}