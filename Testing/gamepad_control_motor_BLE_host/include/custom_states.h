#ifndef CUSTOM_STATES_H
#define CUSTOM_STATES_H

#include "state_machine.h" // Include the base state machine header

enum class Events{
    PAIRED,
    JOY_EVENT,
    BTN_EVENT,
};

StateMachine* create_state_machine();

#endif // CUSTOM_STATES_H