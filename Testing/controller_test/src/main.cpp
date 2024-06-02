#include <Arduino.h>
#include "state_machine.h"
#include "custom_states.h"

#define PWM_L_X 1
#define PWM_L_Y 2
#define PWM_R_X 42
#define PWM_R_Y 41

void serial_recieve_callback();
void send_gamepad_event(int id, int data);

unsigned int t;

void setup() {
  Serial.begin(115200);
  Serial.onReceive(serial_recieve_callback);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PWM_L_X, OUTPUT);
  pinMode(PWM_L_Y, OUTPUT);
  pinMode(PWM_R_X, OUTPUT);
  pinMode(PWM_R_Y, OUTPUT);
  analogWrite(PWM_L_X, 127);
  analogWrite(PWM_L_Y, 127);
  analogWrite(PWM_R_X, 127);
  analogWrite(PWM_R_Y, 127);
  t = 0;
}

void loop() {
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
}

void serial_recieve_callback(){
  digitalWrite(LED_BUILTIN, HIGH);
  if (Serial.read() != 0xAF){
    return;
  }
  int id = Serial.read();
  if (id < 5){ //joy id range
    Serial.print(Serial.read(), HEX);
    Serial.print(" ");
    Serial.print(Serial.read(), HEX);
  }else{
    Serial.print(Serial.read(), HEX);
  }
  // Serial.print("Recieved ");
  // Serial.print(byte_in, HEX);
  // Serial.print(" ");
  // Serial.print(Serial.read(), HEX);
  // Serial.print(" ");
  // if (byte_in < 5){
    // Serial.print(Serial.read(), HEX);
    // Serial.print(" ");
    // Serial.print(Serial.read(), HEX);
  // }else{
    // Serial.print(Serial.read(), HEX);
  // }
  // Serial.print(" ");
  // Serial.println(Serial.read(), HEX);
  //construct the data to make an event
  /*
  int byte_in = Serial.read();
  static int state = 0;
  static uint16_t joy_axis;
  static int id = 0;
  switch (state) {
    case 0: //waiting
      id = 0;
      if (byte_in == 0) break;
      if (byte_in == 0x0A) break;
      if (byte_in < 5){
        //got a joy id
        joy_axis = 0;
        id = byte_in;
        state = 1;
        break;
      }
      if (byte_in > 4){
        //got a btn id
        id = byte_in;
        state = 3;
        break;
      }
      break;
    case 1: //got joy axis (1/2)
      joy_axis |= (byte_in << 8);
      state = 2;
      break;
    case 2: //got joy axis (2/2)
      joy_axis |= byte_in;
      send_gamepad_event(id, joy_axis);
      state = 0;
      break;
    case 3: //got btn state (1/1)
      send_gamepad_event(id, byte_in);
      state = 0;
      break;
  }
  */
}

void send_gamepad_event(int id, int data){
  Serial.print(id);
  Serial.print("-");
  Serial.println(data);
  /*
  switch(id){
    case 0: //invalid id
      break;
    case 1: //Lx
      analogWrite(PWM_L_X, map(data, -30000, 30000, 0, 255));
      break;
    case 2: //Ly
      analogWrite(PWM_L_Y, map(data, -30000, 30000, 0, 255));
      break;
    case 3: //Rx
      analogWrite(PWM_R_X, map(data, -30000, 30000, 0, 255));
      break;
    case 4: //Ry
      analogWrite(PWM_R_Y, map(data, -30000, 30000, 0, 255));
      break;
    case 5: //BTN_NORTH
      break;
    case 6: //BTN_SOUTH
      break;
    case 7: //BTN_EAST
      break;
    case 8: //BTN_WEST
      break;
    case 9: //BTN_SELECT
      break;
    case 10: //BTN_START
      break;
  }*/
}