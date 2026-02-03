#pragma once
#include "MotorDriver.hpp"

enum class MotorState {
  IDLE,
  RUNNING,
  FAULT
};

enum class MotorEvent {
  CMD_START,
  CMD_STOP,
  OVER_CURRENT
};

class MotorStateMachine {
  public:
    explicit MotorStateMachine(MotorDriver& driver)
    :driver_(driver),state_(MotorState::IDLE){}

    void handleEvent(MotorEvent event){
      switch(state_){
        case MotorState::IDLE:
          handleIdle(event);
          break;
        case MotorState::RUNNING:
          handleRunning(event);
          break;
        case MotorState::FAULT:
          handleFault(event);
          break;
      }
    }

    MotorState state() const {return state_;}

  private:
    void handleIdle(MotorEvent event){
      if(event == MotorEvent::CMD_START){
        driver_.start();
        state_ = MotorState::RUNNING;
      }
    }

    void handleRunning(MotorEvent event) {
      if(event == MotorEvent::CMD_STOP){
        driver_.stop();
        state_ = MotorState::IDLE;
      }else if(event == MotorEvent::OVER_CURRENT){
        driver_.stop();
        state_ = MotorState::FAULT;
      }
    }

    void handleFault(MotorEvent event){
      if(event == MotorEvent::CMD_STOP){
        state_ = MotorState::IDLE;
      }
    }

    MotorDriver driver_;
    MotorState state_;
};