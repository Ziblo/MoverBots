#include "custom_states.h"

// DisarmedState class implementation
class DisarmedState : public State {
public:
    void Enter() override {
        
    }

    void OnEvent(Events event) override {
        
    }

    void Exit() override {
        
    }
};

// ArmedState class implementation
class ArmedState : public State {
public:
    void Enter() override {
        
    }

    void OnEvent(Events event) override {
        
    }

    void Exit() override {
        
    }
};

StateMachine* create_state_machine(){
    State* states[] = {
        new DisarmedState(),
        new ArmedState(),
    };
    StateMachine* state_machine = new StateMachine(states, 1);
    return state_machine;
}