#include "max_gamepad.h"
#include "MoverBotsOdrive.h"
#include "gamepad_enums.h"

MaxGamepad::MaxGamepad(){
    Serial.println("Initializing gamepad");
}

void MaxGamepad::callback(){
    digitalWrite(LED_BUILTIN, HIGH);

    // Form: 0xAF{id}{state}0A
    // State can be two bytes if it's a joystick event

    // Check for header byte "0xAF"
    if (Serial.read() != 0xAF) return;

    // Save id
    int id = Serial.read();
    int data;

    if (id < 5) { // Joystick id range
        int joy_axis = get_joy_axis();
        data = joy_axis;
    } else { // Button id range
        int btn_state = Serial.read();
        data = btn_state;
    }

    if (Serial.read() != 0x0A) return;

    // Send event to state machine
    send_gamepad_event(id, data);

    // Do it all again if there's still more data
    if (Serial.available() > 0) {
        callback();
    }
}

int MaxGamepad::get_joy_axis(){
    int joy1 = Serial.read();
    int joy2 = Serial.read();
    int joy = (joy2 << 8) | joy1;

    // Convert to signed 16-bit integer
    if (joy > 32767) {
        joy -= 65536;
    }

    return joy;
}

int MaxGamepad::is_Joy_id(int id){
    switch (id){
        case LEFT_STICK_X:
        case LEFT_STICK_Y:
        case RIGHT_STICK_X:
        case RIGHT_STICK_Y:
            return true;
    }
    return false;
}

// void send_gamepad_event(int id, int data){
//     switch (id){
//     case LEFT_STICK_X:
//       Serial.print("Left Stick X: ");
//       Serial.println(data);
//       break;
//     case LEFT_STICK_Y:
//       Serial.print("Left Stick Y: ");
//       Serial.println(data);
//       break;
//     case RIGHT_STICK_X:
//       Serial.print("Right Stick X: ");
//       Serial.println(data);
//       break;
//     case RIGHT_STICK_Y:
//       Serial.print("Right Stick Y: ");
//       Serial.println(data);
//       break;
//     case BTN_NORTH:
//       Serial.print("North Btn: ");
//       Serial.println(data ? "DOWN" : "UP");
//       break;
//     case BTN_SOUTH:
//       Serial.print("South Btn: ");
//       Serial.println(data ? "DOWN" : "UP");
//       break;
//     case BTN_EAST:
//       Serial.print("East Btn: ");
//       Serial.println(data ? "DOWN" : "UP");
//       break;
//     case BTN_WEST:
//       Serial.print("West Btn: ");
//       Serial.println(data ? "DOWN" : "UP");
//       break;
//     case BTN_START:
//       Serial.print("Start Btn: ");
//       Serial.println(data ? "DOWN" : "UP");
//       break;
//     case BTN_SELECT:
//       Serial.print("Select Btn: ");
//       Serial.println(data ? "DOWN" : "UP");
//       break;
//   }
// }