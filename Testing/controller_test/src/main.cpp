#include <Arduino.h>
#include "state_machine.h"
#include "custom_states.h"

void serial_recieve_callback();

#define TIMER_1_TIME 1000 //1 second

int incomingByte = 0; // for incoming serial data
unsigned long time;

void setup() {
  Serial.begin(115200);
  Serial.onReceive(serial_recieve_callback);
  pinMode(LED_BUILTIN, OUTPUT);
  time = 0;
  //Initialize States
  StateMachine state_machine(nullptr);
}

void loop() {
  unsigned long new_time = millis();
  if (new_time >= TIMER_1_TIME){
    state_machine(TIMEOUT_1);
  }
}

void serial_recieve_callback(){
  digitalWrite(LED_BUILTIN, HIGH);
  incomingByte = Serial.read();
  Serial.print("I received: ");
  Serial.println(incomingByte, DEC);
  state_machine();
}