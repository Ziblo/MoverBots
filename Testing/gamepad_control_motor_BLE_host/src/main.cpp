//includes
#include "MoverBotsODrive.h"
#include "max_gamepad.h"
#include "gamepad_enums.h"
//Bluetooth includes
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "MoverBotHost.h"
#include "custom_UUIDs.h"

//Class overrides
class SMGamepad : public MaxGamepad{
public:
  void send_gamepad_event(int id, int data) override;
};

//function declarations
void serial_recieve_callback();

//global variables
MOdrive swerve_drive;
SMGamepad gamepad;
MoverBotHost* pMoverBotHost;
bool old_is_connected = false;

//Characteristics
BLECharacteristic* gamepad_characteristic;



#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "custom_UUIDs.h"

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristics[NUM_OF_CHARACTERISTICS];
bool deviceConnected = false;
bool oldDeviceConnected = false;

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      Serial.println("Found connection in callback!");
      deviceConnected = true;
      BLEDevice::startAdvertising();
    };

    void onDisconnect(BLEServer* pServer) {
      Serial.println("Disconnect in callback!");
      deviceConnected = false;
    }
};



void setup() {
  Serial.begin(115200);

  // Create the BLE Device
  BLEDevice::init("ESP32 Host");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  ///*
  // Create the Master BLE Service
  BLEService *pMasterService = pServer->createService(SERVICE_UUID);

  // Create all the BLE Characteristics
  for (int i=0; i<NUM_OF_CHARACTERISTICS; i++){
    uint32_t properties = 0;
    switch (MoverBotCharacteristics[i].mode){
      case NOTIFY:
        properties = BLECharacteristic::PROPERTY_NOTIFY;
        break;
      case CALLBACK:
      case TWO_WAY: //Callback and Two-way both use read and write
        properties = BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE;
        break;
      default:
        //UNRECOGNISED MODE
        break;
    }
    pCharacteristics[i] = pMasterService->createCharacteristic(MoverBotCharacteristics[i].UUID, properties);
  }

  // Create all the BLE Descriptors
  BLEDescriptor* pDescriptors[NUM_OF_CHARACTERISTICS]; //Characteristic User Descriptions
  for (int i=0; i<NUM_OF_CHARACTERISTICS; i++){
    pDescriptors[i] = new BLEDescriptor((uint16_t)0x2901); //descriptor for a Client Characteristic Configuration
    pDescriptors[i]->setValue(MoverBotCharacteristics[i].description);
    pCharacteristics[i]->addDescriptor(pDescriptors[i]);
    BLE2902* p2902 = new BLE2902(); //pointer to a generic descriptor for a Client Characteristic Configuration
    p2902->setNotifications(true);
    pCharacteristics[i]->addDescriptor(p2902);
  }
  //*/
  //Make the Master Service
  //BLEService *pMasterService = InitService(pServer, SERVICE_UUID, NUM_OF_CHARACTERISTICS, MoverBotCharacteristics);
  
  // Start the service
  pMasterService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true); //needs this to be true to show service uuid in advertisement
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
  Serial.println("Waiting a client connection to notify...");
}

void loop() {
    // notify changed value
    static uint32_t Collective_Heading = 5;
    static uint32_t num_of_bots = 0;
    if (deviceConnected) {
        //update the characteristics
        Serial.println("Sending characteristics!");
        pCharacteristics[0]->setValue(num_of_bots);
        pCharacteristics[0]->notify();
        Serial.print("sending in characteristic 0: ");
        Serial.println(num_of_bots);

        pCharacteristics[1]->setValue(Collective_Heading);
        pCharacteristics[1]->notify();
        Serial.print("sending in characteristic 1: ");
        Serial.println(Collective_Heading);

        Collective_Heading += 1; //count by 1s
        num_of_bots -=1;

        delay(1000); // bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
    }
    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
}








//SETUP
void setup(){  
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial.onReceive(serial_recieve_callback);
  Serial.println("Creating BLE Host...");
  pMoverBotHost = new MoverBotHost(); //creates a BLE server and inits master service
  Serial.println("BLE Host created");
  gamepad_characteristic = pMoverBotHost->GetCharacteristic(0);
}

//LOOP
void loop(){
  //Check BLE is connection status
  bool is_connected = pMoverBotHost->is_connected();
  int i;
  // connecting
  if (is_connected && !old_is_connected){
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Connecting!");
    i = 0;
  }
  if (is_connected) {
    i++;
    Serial.println("Still Connected!");
    delay(2000);
    gamepad_characteristic->setValue(i);
    gamepad_characteristic->notify();
  }
  // disconnecting
  if (!is_connected && old_is_connected){
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("Disconnecting!");
  }
  old_is_connected = is_connected;
}

//On serial recieve
void serial_recieve_callback(){
  gamepad.callback();
}

//Override for gamepad event (called from gamepad.callback())
void SMGamepad::send_gamepad_event(int id, int data){
  switch (id){
    case LEFT_STICK_X:
      Serial.print("Left Stick X: ");
      Serial.println(data);
      break;
    case LEFT_STICK_Y:
      Serial.print("Left Stick Y: ");
      Serial.println(data);
      break;
    case RIGHT_STICK_X:
      Serial.print("Right Stick X: ");
      Serial.println(data);
      break;
    case RIGHT_STICK_Y:
      Serial.print("Right Stick Y: ");
      Serial.println(data);
      break;
    case BTN_NORTH:
      Serial.print("North Btn: ");
      Serial.println(data ? "DOWN" : "UP");
      break;
    case BTN_SOUTH:
      Serial.print("South Btn: ");
      Serial.println(data ? "DOWN" : "UP");
      break;
    case BTN_EAST:
      Serial.print("East Btn: ");
      Serial.println(data ? "DOWN" : "UP");
      break;
    case BTN_WEST:
      Serial.print("West Btn: ");
      Serial.println(data ? "DOWN" : "UP");
      break;
    case BTN_START:
      Serial.print("Start Btn: ");
      Serial.println(data ? "DOWN" : "UP");
      if (data) swerve_drive.Init();
      break;
    case BTN_SELECT:
      Serial.print("Select Btn: ");
      Serial.println(data ? "DOWN" : "UP");
      if (data) swerve_drive.clear_errors();
      break;
  }
}