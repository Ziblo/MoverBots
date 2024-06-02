#include "max_gamepad.h"

MaxGamepad::MaxGamepad() {
    Serial.begin(115200);
    Serial.onReceive([this]() { this->serial_receive_callback(); });
}

void MaxGamepad::serial_receive_callback() {
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
        serial_receive_callback();
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
        case 1: // Lx
            Serial.print("Left stick X ");
            Serial.println(data);
            break;
        case 2: // Ly
            Serial.print("Left stick Y ");
            Serial.println(data);
            break;
        case 3: // Rx
            Serial.print("Right stick X ");
            Serial.println(data);
            break;
        case 4: // Ry
            Serial.print("Right stick Y ");
            Serial.println(data);
            break;
        case 5: // BTN_NORTH
            Serial.println(data ? "North Btn DOWN" : "North Btn UP");
            break;
        case 6: // BTN_SOUTH
            Serial.println(data ? "South Btn DOWN" : "South Btn UP");
            break;
        case 7: // BTN_EAST
            Serial.println(data ? "East Btn DOWN" : "East Btn UP");
            break;
        case 8: // BTN_WEST
            Serial.println(data ? "West Btn DOWN" : "West Btn UP");
            break;
        case 9: // BTN_SELECT
            Serial.println(data ? "Select Btn DOWN" : "Select Btn UP");
            break;
        case 10: // BTN_START
            Serial.println(data ? "Start Btn DOWN" : "Start Btn UP");
            break;
    }
}
