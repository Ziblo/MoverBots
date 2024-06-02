#include <Arduino.h>
#include "state_machine.h"
#include "custom_states.h"

#define PWM_L_X 1
#define PWM_L_Y 2
#define PWM_R_X 42
#define PWM_R_Y 41

void serial_recieve_callback();
void send_gamepad_event(int id, int data);
int get_joy_axis();

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
  delay(5000);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("beans");
}

int get_joy_axis(){
  int joy1 = Serial.read();
  int joy2 = Serial.read();
  int joy = (joy2 << 8) | joy1;
    // Convert to signed 16-bit integer
    if (joy > 32767) {
        joy -= 65536;
    }
    return joy;
}

void serial_recieve_callback(){
  digitalWrite(LED_BUILTIN, HIGH);
  //form: 0xAF{id}{state}0A
  //state can be two bytes if its a joystick event
  
  //check for header byte "0xAF"
  if (Serial.read() != 0xAF) return;
  //save id
  int id = Serial.read();
  int data;
  if (id < 5){ //joy id range
    int joy_axis = get_joy_axis();
    data = joy_axis;
  }else{ //button id range
    int btn_state = Serial.read();
    data = btn_state;
  }
  if (Serial.read() != 0x0A) return;
  
  //send event to state machine
  send_gamepad_event(id, data);

  //do it all again if there's still more data
  if (Serial.available() > 0){
    serial_recieve_callback();
  }
}

void send_gamepad_event(int id, int data){
  switch(id){
    case 0: //invalid id
      break;
    case 1: //Lx
      analogWrite(PWM_L_X, map(data, -32767, 32767, 0, 255));
      Serial.print("Left stick X ");
      Serial.println(data);
      break;
    case 2: //Ly
      analogWrite(PWM_L_Y, map(data, -32767, 32767, 0, 255));
      Serial.print("Left stick Y ");
      Serial.println(data);
      break;
    case 3: //Rx
      analogWrite(PWM_R_X, map(data, -32767, 32767, 0, 255));
      Serial.print("Right stick X ");
      Serial.println(data);
      break;
    case 4: //Ry
      analogWrite(PWM_R_Y, map(data, -32767, 32767, 0, 255));
      Serial.print("Right stick Y ");
      Serial.println(data);
      break;
    case 5: //BTN_NORTH
      Serial.println(data ? "North Btn DOWN" : "North Btn UP");
      break;
    case 6: //BTN_SOUTH
      Serial.println(data ? "South Btn DOWN" : "South Btn UP");
      break;
    case 7: //BTN_EAST
      Serial.println(data ? "East Btn DOWN" : "East Btn UP");
      break;
    case 8: //BTN_WEST
      Serial.println(data ? "West Btn DOWN" : "West Btn UP");
      break;
    case 9: //BTN_SELECT
      Serial.println(data ? "Select Btn DOWN" : "Select Btn UP");
      break;
    case 10: //BTN_START
      Serial.println(data ? "Start Btn DOWN" : "Start Btn UP");
      break;
  }
}