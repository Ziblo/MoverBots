#ifndef MAX_GAMEPAD_H
#define MAX_GAMEPAD_H

#include <Arduino.h>
#include "MoverBotsOdrive.h"

class MaxGamepad {
public:
    MaxGamepad();
    void callback();
    int get_joy_axis();
    virtual void send_gamepad_event(int id, int data) = 0;
};

#endif // MAX_GAMEPAD_H
