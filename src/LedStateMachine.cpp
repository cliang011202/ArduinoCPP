#include "LedStateMachine.hpp"

LedStateMachine::LedStateMachine(Led& led)
  :led_(led)
  ,state_(LedState::OFF)
{
  led_.off();
}

void LedStateMachine::update(bool timeout_event)
{
  if(!timeout_event) return;

  switch(state_){
    case LedState::OFF:
      led_.on();
      state_ = LedState::ON;
      break;
    case LedState::ON:
      led_.off();
      state_ = LedState::OFF;
      break;
  }
}