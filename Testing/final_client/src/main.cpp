/**
 * BLE client 
 * There is a lot new capabilities implemented.
 * author unknown
 * updated by chegewara
 */

#include <Arduino.h>
#include <BLEDevice.h>
#include "custom_UUIDs.h"
#include "gamepad_enums.h"
#include "MoverBotsODrive.h"

//Glbal variables
MOdrive swerve_drive;

// The remote service we wish to connect to.
static BLEUUID serviceUUID(SERVICE_UUID);
static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;
static BLEAdvertisedDevice* myDevice;

//function prototypes
bool connectCharacteristic(BLERemoteService* pRemoteService, BLEUUID local_characteristic_UUID);
void PrintCharacteristic(uint8_t* pData, size_t length, CharacteristicType dataType);
void sendGamepadInput(int id, int data);
int getCharData(uint8_t* pData, size_t length, CharacteristicType dataType);

static void notifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify){
  //called everytime the server does a notify to its clients
  int i; //get the index
  bool found_characteristic = false;
  for (i=0; i<NUM_OF_CHARACTERISTICS; i++){
    if (pBLERemoteCharacteristic->getUUID().toString() == MoverBotCharacteristics[i].UUID){
      found_characteristic = true;
      break;
    }
  }
  if (!found_characteristic){
    Serial.print("Notify callback was not matched to a known characteristicUUID");
    return;
  }
  //print info
  // Serial.print("Notify callback for characteristic ");
  // Serial.print(MoverBotCharacteristics[i].description);
  // Serial.print(" of data length ");
  // Serial.print(length);
  // Serial.print(" Data: ");
  static int id;
  int value;
  switch (i){
    case 0: //characteristicUUIDs[0]:
      id = getCharData(pData, length, MoverBotCharacteristics[0].type);
      break;
    case 1: //characteristicUUIDs[1]:
      value = getCharData(pData, length, MoverBotCharacteristics[1].type);
      sendGamepadInput(id, value);
      break;
    case 2: //characteristicUUIDs[2]:
      break;
    default: //pBLERemoteCharacteristic doesn't match my characteristicUUIDs
      break;
  }
  //PrintCharacteristic(pData, length, MoverBotCharacteristics[i].type);
}

int getCharData(uint8_t* pData, size_t length, CharacteristicType dataType){
  uint32_t number;
  switch (dataType){
    case INTEGER:
      //print as uint32
      number = pData[0]; //start at lsb
      for (int i=1; i<length; i++){
        //shift bytes
        number = number | (pData[i]<<i*8);
      }
      return number;
      break;
    default:
      Serial.println("Type is not recognised.");
      return 0;
      break;
  }
}

void PrintCharacteristic(uint8_t* pData, size_t length, CharacteristicType dataType){
  uint32_t number;
  switch (dataType){
    case INTEGER:
      //print as uint32
      number = pData[0]; //start at lsb
      for (int i=1; i<length; i++){
        //shift bytes
        number = number | (pData[i]<<i*8);
      }
      Serial.println(number);
      break;
    case STRING:
      //print as string
      for (int i=0; i<length; i++){
        Serial.print((char)pData[i]);
      }
      Serial.print("\n");
      break;
    default:
      Serial.println("Type is not recognised.");
      break;
  }
}

class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
    //Don't stop advertising!
  }
  void onDisconnect(BLEClient* pclient) {
    connected = false;
    Serial.println("Disconnected");
  }
};

bool connectToServer() {
  Serial.print("Forming a connection to ");
  Serial.println(myDevice->getAddress().toString().c_str());
  
  BLEClient*  pClient  = BLEDevice::createClient();
  Serial.println(" - Created client");

  pClient->setClientCallbacks(new MyClientCallback());

  // Connect to the remove BLE Server.
  pClient->connect(myDevice);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
  Serial.println(" - Connected to server");
  pClient->setMTU(517); //set client to request maximum MTU from server (default is 23 otherwise)

  // Obtain a reference to the service we are after in the remote BLE server.
  BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
  if (pRemoteService == nullptr) {
    Serial.print("Failed to find our service UUID: ");
    Serial.println(serviceUUID.toString().c_str());
    pClient->disconnect();
    return false;
  }
  Serial.println(" - Found our service");
  connected = true;

  //check for expected characteristics
  for (int i=0; i<NUM_OF_CHARACTERISTICS && connected; i++){
    connected = connectCharacteristic(pRemoteService, BLEUUID(MoverBotCharacteristics[i].UUID));
    if (!connected){
      pClient->disconnect();
      Serial.println("At least one characteristic UUID not found");
      return false;
    }
  }
  return true;
}

bool connectCharacteristic(BLERemoteService* pRemoteService, BLEUUID local_characteristic_UUID) {
  // Obtain a reference to the characteristic in the service of the remote BLE server.
  BLERemoteCharacteristic* pRemoteCharacteristic;
  pRemoteCharacteristic = pRemoteService->getCharacteristic(local_characteristic_UUID);
  if (pRemoteCharacteristic == nullptr) {
    Serial.print("Failed to find our characteristic UUID: ");
    Serial.println(local_characteristic_UUID.toString().c_str());
    return false;
  }
  Serial.println(" - Found our characteristic");

  // Don't need to read the value of the characteristic right now.
  /*if(pRemoteCharacteristic->canRead()) {
    std::string value = pRemoteCharacteristic->readValue();
    Serial.print("The characteristic value was: ");
    Serial.println(value.c_str());
  }*/

  if(pRemoteCharacteristic->canNotify()) {
    pRemoteCharacteristic->registerForNotify(notifyCallback);
  }
  return true;
}

// Scan for BLE servers and find the first one that advertises the service we are looking for.
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
 /**
   * Called for each advertising BLE server.
   */
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print("BLE Advertised Device found: ");
    //Serial.println(advertisedDevice.toString().c_str());
    ///*
    Serial.print("Name: ");
    Serial.print(advertisedDevice.getName().c_str());
    Serial.print(" Service UUID: ");
    Serial.println(advertisedDevice.getServiceUUID().toString().c_str());//*/

    // We have found a device, let us now see if it contains the service we are looking for.
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {
      Serial.println("FOUND DEVICE!~");
      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan = true;

    } // Found our server
  } // onResult
}; // MyAdvertisedDeviceCallbacks

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial.println("Starting Arduino BLE Client application...");
  BLEDevice::init("");

  // Retrieve a Scanner and set the callback we want to use to be informed when we
  // have detected a new device.  Specify that we want active scanning and start the
  // scan to run for 5 seconds.
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5, false);
} // End of setup.


// This is the Arduino main loop function.
void loop() {

  // If the flag "doConnect" is true then we have scanned for and found the desired
  // BLE Server with which we wish to connect.  Now we connect to it.  Once we are 
  // connected we set the connected flag to be true.
  if (doConnect == true) {
    if (connectToServer()) {
      Serial.println("We are now connected to the BLE Server.");
    } else {
      Serial.println("We have failed to connect to the server; there is nothin more we will do.");
    }
    doConnect = false;
  }
  // If we are connected to a peer BLE Server, update the characteristic each time we are reached
  // with the current time since boot.
  if (connected) {
    digitalWrite(LED_BUILTIN, HIGH);
    // String newValue = "Time since boot: " + String(millis()/1000);
    // Serial.println("Setting new characteristic value to \"" + newValue + "\"");
    
    // Set the characteristic's value to be the array of bytes that is actually a string.
    // pRemoteCharacteristic->writeValue(newValue.c_str(), newValue.length());
  }else if(doScan){
    BLEDevice::getScan()->start(0);  // this is just example to start scan after disconnect, most likely there is better way to do it in arduino
    digitalWrite(LED_BUILTIN, LOW);
  }else{ //disconnected
    digitalWrite(LED_BUILTIN, LOW);
    swerve_drive.set_m1_speed(0);
    swerve_drive.set_m2_speed(0);
  }
  
  delay(1000); // Delay a second between loops.
} // End of loop

void sendGamepadInput(int id, int data){
  switch (id){
    case LEFT_STICK_X:
      Serial.print("Left Stick X: ");
      Serial.println(data);
      break;
    case LEFT_STICK_Y:
      Serial.print("Left Stick Y: ");
      Serial.println(data);
      swerve_drive.set_m1_speed(VELOCITY_LIMIT*data/32767);
      break;
    case RIGHT_STICK_X:
      Serial.print("Right Stick X: ");
      Serial.println(data);
      break;
    case RIGHT_STICK_Y:
      Serial.print("Right Stick Y: ");
      Serial.println(data);
      swerve_drive.set_m2_speed(VELOCITY_LIMIT*data/32767);
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