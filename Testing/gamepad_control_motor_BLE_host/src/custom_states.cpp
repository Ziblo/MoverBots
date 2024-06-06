#include "custom_states.h"
#include <Arduino.h>


// PairingState class implementation
class PairingState : public State {
public:
    String name = "Pairing";
    void Enter() override {
        Serial.print("Entering ");
        Serial.print(name);
        Serial.println(" State");
    }

    void OnEvent(Events event) override {
        
    }

    void Exit() override {
        Serial.print("Exiting Pairing State");
    }
};


// ControlState class implementation
class ControlState : public State {
public:
    String name = "Control";
    void Enter() override {
        Serial.print("Entering ");
        Serial.print(name);
        Serial.println(" State");
    }

    void OnEvent(Events event) override {
        
    }

    void Exit() override {
        
    }
};


// FeedbackState class implementation
class FeedbackState : public State {
public:
    String name = "Feedback";
    void Enter() override {
        Serial.print("Entering ");
        Serial.print(name);
        Serial.println(" State");
    }

    void OnEvent(Events event) override {
        
    }

    void Exit() override {
        
    }
};


//Init states
StateMachine* create_state_machine(){
    State* states[] = {
        new PairingState(),
        new ControlState(),
        new FeedbackState(),
    };
    StateMachine* state_machine = new StateMachine(states, 1);
    return state_machine;
}