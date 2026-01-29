#include "Led.hpp"
// #include "LedStateMachine.hpp"

constexpr uint8_t LED_PIN = 13;

enum class LedState {
  OFF,
  ON
};

Led led(LED_PIN);
LedState led_state = LedState::OFF;

void updateLedStateMachine(bool timeout_event);
void taskLed();
void taskDebug();

void setup() {
  Serial.begin(9600);
}

void loop() {
  taskLed();
  taskDebug();
}

void taskLed() {
  static unsigned long last_time = 0;
  constexpr unsigned long INTERVAL_MS = 1000;
  
  unsigned long now = millis();
  if(now - last_time >= INTERVAL_MS){
    last_time = now;
    updateLedStateMachine(true);
  }
}

void taskDebug() {
  static unsigned long last_time = 0;
  constexpr unsigned long INTERVAL_MS = 2000;
  
  unsigned long now = millis();
  if(now - last_time >= INTERVAL_MS){
    last_time = now;
    Serial.println("System alive");
  }
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