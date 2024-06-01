#include "state_machine.h"
#include <cstddef>

// StateMachine class implementation
StateMachine::StateMachine(State** states, size_t num_states) {
    // Handle invalid input
    if (!num_states || !states) return;
    
    // Initialize all states
    for (size_t i = 0; i < num_states; ++i) {
        if (states[i]) {
            states[i]->Init();
        }
    }
}

void StateMachine::Init(State* initial_state){
    // Set the initial state
    current_state = initial_state;
}

void StateMachine::OnEvent(Events event) {
    if (current_state) {
        current_state->OnEvent(event); // Pass the event to the current state
    }
}

void StateMachine::Transition(State* new_state) {
    if (new_state == current_state) {
        return; // No transition needed if the new state is the same as the current state
    }
    if (current_state) {
        current_state->Exit(); // Exit the current state
    }
    new_state->Enter(); // Enter the new state
    current_state = new_state; // Update the current state
}
