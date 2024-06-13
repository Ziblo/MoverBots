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

  // bool is_Joy_id() const{
  //   switch (id){
  //     case LEFT_STICK_X:
  //     case LEFT_STICK_Y:
  //     case RIGHT_STICK_X:
  //     case RIGHT_STICK_Y:
  //       return true;
  //   }
  //   return false;
  // }


//Classes
class GamepadEvent{
public:
  int id;
  int data;
  GamepadEvent(int id, int data) : id(id), data(data) {}
};

class JoyEvent : public GamepadEvent{
public:
  bool updateFlag;
  JoyEvent(int id, int data) : GamepadEvent(id, data), updateFlag(false) {}
  void clear(){
    GamepadEvent::data = 0;
    updateFlag = false; //?
  }
  void update(int data){
    GamepadEvent::data = data;
    updateFlag = true;
  }
};

class GamepadQueues{
public:
  JoyEvent Lx;
  JoyEvent Ly;
  JoyEvent Rx;
  JoyEvent Ry;
  std::queue<GamepadEvent*> btn_queue;
  GamepadQueues() : Lx(LEFT_STICK_X, 0), Ly(LEFT_STICK_Y, 0), Rx(RIGHT_STICK_X, 0), Ry(RIGHT_STICK_Y, 0) {}
  void clear_all(){
    Lx.clear();
    Ly.clear();
    Rx.clear();
    Ry.clear();
    while(!btn_queue.empty()) btn_queue.pop();
  }
  void push(int id, int data){
    switch (id){
      case LEFT_STICK_X:
        Lx.update(data);
        break;
      case LEFT_STICK_Y:
        Ly.update(data);
        break;
      case RIGHT_STICK_X:
        Rx.update(data);
        break;
      case RIGHT_STICK_Y:
        Ry.update(data);
        break;
      default: //btn event
        btn_queue.push(new GamepadEvent(id, data));
        break;
    }
  }
  bool empty() const {
    bool has_val = false;
    has_val |= Lx.updateFlag;
    has_val |= Ly.updateFlag;
    has_val |= Rx.updateFlag;
    has_val |= Ry.updateFlag;
    has_val |= !btn_queue.empty();
    return !has_val;
  }
  GamepadEvent* pop(){
    static int index = 0;
    GamepadEvent* out = nullptr;
    while (!out){
      switch (index){
        case 0:
          if (Lx.updateFlag) {
            out = &Lx;
            Lx.updateFlag = false;
          }
          break;
        case 1:
          if (Ly.updateFlag) {
            out = &Ly;
            Ly.updateFlag = false;
          }
          break;
        case 2:
          if (Rx.updateFlag) {
            out = &Rx;
            Rx.updateFlag = false;
          }
          break;
        case 3:
          if (Ry.updateFlag) {
            out = &Ry;
            Ry.updateFlag = false;
          }
          break;
        case 4:
        case 5:
          //btn gets two chances
          if (!btn_queue.empty()){
            out = btn_queue.front();
            btn_queue.pop();
          }
          break;
      }
      index = (index+1)%6;
    }
    return out;
  }
};

//Class overrides
class SMGamepad : public MaxGamepad{
public:
  void send_gamepad_event(int id, int data) override;
};

//function declarations
void serial_recieve_callback();

//global variables
SMGamepad gamepad;
GamepadQueues gamepad_queues;

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

class MyDebugCallback : public BLECharacteristicCallbacks {
public:
    void onWrite(BLECharacteristic *pChar) override;
};

void MyDebugCallback::onWrite(BLECharacteristic *pChar){
  //debug_print
  std::string str = pChar->getValue();
  Serial.print("Bot: ");
  Serial.println(str.c_str());
}

//SETUP
void setup(){  
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
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

  Serial.println("Adding Callbacks...");
    // Now can add the callback function here if needed
    for (int i=0; i<NUM_OF_CHARACTERISTICS; i++){
      switch (MoverBotCharacteristics[i].mode){
          case CALLBACK:
            pCharacteristics[i]->setCallbacks(new MyDebugCallback());
            break;
      }
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
    if (!gamepad_queues.empty()){
      GamepadEvent* gp_e = gamepad_queues.pop();
      pCharacteristics[0]->setValue(gp_e->id);
      pCharacteristics[0]->notify();
      pCharacteristics[1]->setValue(gp_e->data);
      pCharacteristics[1]->notify();
    }
    // if (!joy_queue.empty()){
    //   GamepadEvent* gp_e = joy_queue.front();
    //   joy_queue.pop();
    //   while (!joy_queue.empty() && (joy_queue.front()->id == gp_e->id)){
    //     gp_e = joy_queue.front();
    //     joy_queue.pop();
    //   }
    //   pCharacteristics[0]->setValue(gp_e->id);
    //   pCharacteristics[0]->notify();
    //   pCharacteristics[1]->setValue(gp_e->data);
    //   pCharacteristics[1]->notify();
    // }
    // delay(5);
    // if(!btn_queue.empty()){
    //   GamepadEvent* gp_e = btn_queue.front();
    //   btn_queue.pop();
    //   pCharacteristics[0]->setValue(gp_e->id);
    //   pCharacteristics[0]->notify();
    //   pCharacteristics[1]->setValue(gp_e->data);
    //   pCharacteristics[1]->notify();
    // }
    delay(5); // bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
  }
  // disconnecting
  if (!deviceConnected && oldDeviceConnected) {
    //clear gamepad_queues
    gamepad_queues.clear_all();
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
    // int* gp_e = new int[2]{id, data};
    // GamepadEvent* gp_e = new GamepadEvent(id, data);
    gamepad_queues.push(id, data);
    // if (gp_e->is_Joy_id()){
    //   joy_queue.push(gp_e);
    // }else{
    //   btn_queue.push(gp_e);
    // }
  }else{
    Serial.println("Unable to send gamepad event; no devices connected.");
  }
}