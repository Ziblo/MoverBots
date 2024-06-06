// includes
#include "MoverBotsODrive.h"
#include "max_gamepad.h"
#include "gamepad_enums.h"
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

//SETUP
void setup(){  
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial.onReceive(serial_recieve_callback);
  gamepad.set_odrive(&swerve_drive);
}

//LOOP
void loop(){
  delay(5000);
  Serial.println("beans");
  // digitalWrite(LED_BUILTIN, LOW);
}

//On serial recieve
void serial_recieve_callback(){
  gamepad.callback();
}

//On gamepad_button recieved (called by gamepad.callback())
void SMGamepad::send_gamepad_event(int id, int data){
  switch (id){
    case LEFT_STICK_X:
      break;
    case LEFT_STICK_Y:
      break;
    case RIGHT_STICK_X:
      break;
    case RIGHT_STICK_Y:
      break;
    case BTN_NORTH:
      break;
    case BTN_SOUTH:
      break;
    case BTN_EAST:
      break;
    case BTN_WEST:
      break;
    case BTN_START:
      break;
    case BTN_SELECT:
      break;
  }
}