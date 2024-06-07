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

//SETUP
void setup(){  
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial.onReceive(serial_recieve_callback);
  Serial.println("Creating BLE Host...");
  pMoverBotHost = new MoverBotHost(); //creates a BLE server and inits master service
}

//LOOP
void loop(){
  //Check BLE is connection status
  bool is_connected = pMoverBotHost->is_connected();
  // connecting
  if (is_connected && !old_is_connected){
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Connecting!");
  }
  if (is_connected) {
    Serial.println("Still Connected!");
    delay(2000);
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