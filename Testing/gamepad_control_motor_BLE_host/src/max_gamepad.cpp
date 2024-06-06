#include "max_gamepad.h"
#include "MoverBotsOdrive.h"
#include "gamepad_enums.h"

MaxGamepad::MaxGamepad() {
    Serial.println("Initializing gamepad");
}

void MaxGamepad::callback() {
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

int MaxGamepad::get_joy_axis() {
    int joy1 = Serial.read();
    int joy2 = Serial.read();
    int joy = (joy2 << 8) | joy1;

    // Convert to signed 16-bit integer
    if (joy > 32767) {
        joy -= 65536;
    }

    return joy;
}

void MaxGamepad::send_gamepad_event(int id, int data) {
    switch (id) {
        case 0: // Invalid id
            break;
        case LEFT_STICK_X: // Lx
            Serial.print("Left stick X ");
            Serial.println(data);
            break;
        case LEFT_STICK_Y: // Ly
            Serial.print("Left stick Y ");
            Serial.println(data);
            if (p_odrive) p_odrive->set_m1_speed(VELOCITY_LIMIT*data/32767);
            break;
        case RIGHT_STICK_X: // Rx
            Serial.print("Right stick X ");
            Serial.println(data);
            break;
        case RIGHT_STICK_Y: // Ry
            Serial.print("Right stick Y ");
            Serial.println(data);
            if (p_odrive) p_odrive->set_m2_speed(VELOCITY_LIMIT*data/32767);
            break;
        case BTN_NORTH: // BTN_NORTH
            Serial.println(data ? "North Btn DOWN" : "North Btn UP");
            break;
        case BTN_SOUTH: // BTN_SOUTH
            Serial.println(data ? "South Btn DOWN" : "South Btn UP");
            break;
        case BTN_EAST: // BTN_EAST
            Serial.println(data ? "East Btn DOWN" : "East Btn UP");
            break;
        case BTN_WEST: // BTN_WEST
            Serial.println(data ? "West Btn DOWN" : "West Btn UP");
            break;
        case BTN_START: // BTN_START
            Serial.println(data ? "Start Btn DOWN" : "Start Btn UP");
            if (p_odrive && data) p_odrive->Init();
            break;
        case BTN_SELECT: // BTN_SELECT
            Serial.println(data ? "Select Btn DOWN" : "Select Btn UP");
            if (p_odrive && data) p_odrive->clear_errors();
            break;
    }
}

void MaxGamepad::set_odrive(MOdrive* p){
    p_odrive = p;
}