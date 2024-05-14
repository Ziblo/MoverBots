#include <Arduino.h>
#include <list>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "MoverBotHost.h"
#include "custom_UUIDs.h"

MoverBotHost* pMoverBotHost;
bool old_is_connected = false;

void setup() {
  Serial.begin(115200);
  Serial.println("Creating BLE Host...");
  pMoverBotHost = new MoverBotHost();
  Serial.println("Starting loop...");
}

void loop() {
  bool is_connected = pMoverBotHost->is_connected();
  if (is_connected && !old_is_connected){
    Serial.println("Connecting!");
  }
  if (is_connected) {
    Serial.println("Still connected!");
    delay(20);
  }
  // disconnecting
  if (!is_connected && old_is_connected){
    Serial.println("Disconnecting!");
  }
  old_is_connected = is_connected;
}