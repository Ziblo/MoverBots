// includes
#include "MoverBotsODrive.h"
#include "max_gamepad.h"

MOdrive swerve_drive;
MaxGamepad gamepad;

void serial_recieve_callback();

void setup(){  
  // swerve_drive.Init();
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial.onReceive(serial_recieve_callback);
  gamepad.set_odrive(&swerve_drive);
}

void loop(){
  delay(2000);
  Serial.println("beans");
  digitalWrite(LED_BUILTIN, LOW);
}

void serial_recieve_callback(){
  gamepad.callback();
}