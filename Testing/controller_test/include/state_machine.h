#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

// Enum class for events
enum class Events;

// Class representing individual states
class State {
public:
    State(); //constructor
    virtual ~State(); //destructor
    virtual void Init(); //Runs when state machine is being init
    virtual void Enter(); //on entry of state
    virtual void OnEvent(Events event); //every time an event is given to this state
    virtual void Exit(); //on exit of state
};

// Class managing state transitions and event handling
class StateMachine {
public:
    State* current_state = nullptr;
    StateMachine(State** states, size_t num_states);
    void Init(State* initial_state);
    void OnEvent(Events event);
    void Transition(State* new_state);
};

#endif // STATE_MACHINE_H
