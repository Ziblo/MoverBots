#ifndef CUSTOM_STATES_H
#define CUSTOM_STATES_H

#include "state_machine.h" // Include the base state machine header
enum class Events{
    TIMER_1_TIMEOUT,
    TIMER_2_TIMEOUT,
    TIMER_3_TIMEOUT,
    TIMER_4_TIMEOUT,
    SERIAL_RECIEVED,
}



#endif // CUSTOM_STATES_H