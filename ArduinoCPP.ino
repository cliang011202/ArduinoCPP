#include "Led.hpp"
#include "LedStateMachine.hpp"

constexpr uint8_t LED_PIN = 13;
constexpr unsigned long INTERVAL_MS = 500;

Led led(LED_PIN);
LedStateMachine led_fsm(led);

void setup() {
  Serial.begin(9600);
}

void loop() {
  static unsigned long last_time = 0;

  unsigned long now = millis();
  bool timeout_event = false;

  if(now - last_time >= INTERVAL_MS){
    last_time = now;
    timeout_event = true;
  }

  led_fsm.(timeout_event);
}