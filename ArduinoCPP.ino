#include "Led.hpp"

constexpr uint8_t LED_PIN = 13;
// constexpr unsigned long TOGGLE_INTERVAL_MS = 500;
enum class LedState {
  OFF,
  ON
};

Led led(LED_PIN);
LedState led_state = LedState::OFF;

void setup() {
  Serial.begin(9600);
}

void loop() {
  static unsigned long last_time = 0;
  constexpr unsigned long INTERVAL_MS = 1000;

  unsigned long now = millis();
  bool timeout_event = false;

  if(now - last_time >= INTERVAL_MS){
    last_time = now;
    timeout_event = true;
  }

  updateLedStateMachine(timeout_event);
}

void updateLedStateMachine(bool timeout_event){
  if(!timeout_event) return;

  switch (led_state) {
    case LedState::OFF:
      led.on();
      led_state = LedState::ON;
      break;
    case LedState::ON:
      led.off();
      led_state = LedState::OFF;
      break;
  }
}