//includes
#include "max_gamepad.h"
#include "gamepad_enums.h"
//Bluetooth includes
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "custom_UUIDs.h"
#include <queue>

//Class overrides
class SMGamepad : public MaxGamepad{
public:
  void send_gamepad_event(int id, int data) override;
};

//function declarations
void serial_recieve_callback();

//global variables
SMGamepad gamepad;
std::queue<int*> gamepad_queue;

//BLE Host code
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
      deviceConnected = false;
    }
};

//SETUP
void setup(){  
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial.onReceive(serial_recieve_callback);
  Serial.println("Creating BLE Host...");
  // Create the BLE Device
  BLEDevice::init("MoverBotsHost");

  Serial.println("Creating BLE Server...");
  // Create the BLE Server
  pServer = BLEDevice::createServer();
  Serial.println("Setting Server Callbacks...");
  pServer->setCallbacks(new MyServerCallbacks());

  Serial.println("Creating BLE Service...");
  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  Serial.println("Creating BLE Characteristics...");
  // Create all the BLE Characteristics
  for (int i=0; i<NUM_OF_CHARACTERISTICS; i++){
    uint32_t properties = 0;
    switch (MoverBotCharacteristics[i].mode){
      case NOTIFY:
        properties = BLECharacteristic::PROPERTY_NOTIFY;
        Serial.println("Set Notify");
        break;
      case CALLBACK:
        properties = BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE;
        break;
      case TWO_WAY: //Callback and Two-way both use read and write
        properties = BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE;
        break;
      default:
        //UNRECOGNISED MODE
        break;
    }
    pCharacteristics[i] = pService->createCharacteristic(MoverBotCharacteristics[i].UUID, properties);
  }

  Serial.println("Adding Descriptors...");
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
  
  Serial.println("Starting Service...");
  // Start the service
  pService->start();

  Serial.println("Starting Advertisement...");
  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true); //needs this to be true to show service uuid in advertisement
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
  Serial.println("Waiting a client connection to notify...");
}

//LOOP
void loop() {
  // notify gamepad events to client
  if (deviceConnected && !oldDeviceConnected){
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Connecting!");
  }
  if (deviceConnected) {
    //update the characteristics
    if (!gamepad_queue.empty()){
      int* gp_e = gamepad_queue.front();
      gamepad_queue.pop();
      // while ((gamepad_queue.front()[0] == gp_e[0]) && gamepad.is_Joy_id(gamepad_queue.front()[0])){
      //   //Front has same ID and are both Joystick events
      //   //Joystick can be overridden by a more recent value
      //   gp_e = gamepad_queue.front();
      //   gamepad_queue.pop();
      // }
      pCharacteristics[0]->setValue(gp_e[0]);
      pCharacteristics[0]->notify();
      pCharacteristics[1]->setValue(gp_e[1]);
      pCharacteristics[1]->notify();
    }
    delay(5); // bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
  }
  // disconnecting
  if (!deviceConnected && oldDeviceConnected) {
    //clear gamepad_queue
    while (!gamepad_queue.empty()) {
      gamepad_queue.pop();
    }
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("Disconnecting!");
    delay(500); // give the bluetooth stack the chance to get things ready
    pServer->startAdvertising(); // restart advertising
    Serial.println("start advertising");
  }
  oldDeviceConnected = deviceConnected;
}

//On serial recieve
void serial_recieve_callback(){
  gamepad.callback();
}

//Override for gamepad event (called from gamepad.callback())
void SMGamepad::send_gamepad_event(int id, int data){
  if (deviceConnected){
    Serial.print("Queueing Gamepad Event: (");
    Serial.print(id);
    Serial.print(", ");
    Serial.print(data);
    Serial.println(")");
    int* gp_e = new int[2]{id, data};
    gamepad_queue.push(gp_e);
  }else{
    Serial.println("Unable to send gamepad event; no devices connected.");
  }
  /*switch (id){
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
  }*/
}