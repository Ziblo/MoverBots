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
  int joy = (joy1 << 8) | joy2;
    // Convert to signed 16-bit integer
    if (joy > 32767) {
        joy -= 65536;
    }
    return joy;
}

void serial_recieve_callback(){
  digitalWrite(LED_BUILTIN, HIGH);
  if (Serial.read() != 0xAF){
    Serial.println("Not 0xAF");
    return;
  }
  Serial.print("0xAF ");
  int id = Serial.read();
  Serial.print(id, HEX);
  Serial.print(" ");
  if (id < 5){ //joy id range
    int joy_axis = get_joy_axis();
    Serial.print(joy_axis, HEX);
  }else{ //button id range
    int btn_state = Serial.read();
    Serial.print(btn_state, HEX);
  }
  if (Serial.read() != 0x0A){
    Serial.println(" No 0x0A at the end.");
    return;
  }
  Serial.println(" 0A");
  if (Serial.available() > 0){
    serial_recieve_callback();
  }
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