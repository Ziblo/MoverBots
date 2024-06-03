#ifndef MAX_GAMEPAD_H
#define MAX_GAMEPAD_H

#include <Arduino.h>
#include "MoverBotsOdrive.h"

class MaxGamepad {
public:
    MOdrive* p_odrive = nullptr;
    MaxGamepad();
    void callback();
    int get_joy_axis();
    void send_gamepad_event(int id, int data);
    void set_odrive(MOdrive* p);
};

#endif // MAX_GAMEPAD_H
