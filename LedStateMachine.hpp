#pragma once
#include "Led.hpp"

enum class LedState {
  OFF,
  ON
};

class LedStateMachine {
public:
  explicit LedStateMachine(Led& led);

  void update(bool timeout_event);

private:
  Led& led_;
  LedState state_;
};