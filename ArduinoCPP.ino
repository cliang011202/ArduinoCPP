#include "Led.hpp"
//=====================================
// 常量定义
//=====================================
constexpr uint8_t LED_PIN = 13;
constexpr unsigned long LED_INTERVAL_MS = 10000;
//=====================================
// 状态&事件定义
//=====================================
enum class LedState {
  OFF,
  ON
};
enum class Event {
  NONE,
  TIMEOUT,
  FORCE_OFF,
};
//=====================================
// 全局对象&状态
//=====================================
Led led(LED_PIN);
LedState led_state = LedState::OFF;
//=====================================
// 动作函数（Action）
//=====================================
void actionLedOn() {
  led.on();
}

void actionLedOff() {
  led.off();
}
// void updateLedStateMachine(Event event);
// void taskLed();
// void taskDebug();
// void updateStateMachine(Event event);
//=====================================
// 状态迁移表定义
//=====================================
struct Transition {
  LedState current;
  Event event;
  LedState next;
  void (*action)();
};

constexpr Transition transitions[] {
  { LedState::OFF, Event::TIMEOUT, LedState::ON, actionLedOn},
  { LedState::ON, Event::TIMEOUT, LedState::OFF, actionLedOff},
  { LedState::ON, Event::FORCE_OFF, LedState::OFF, actionLedOff},
};

constexpr size_t TRANSITION_COUNT = sizeof(transitions) / sizeof(transitions[0]);

//=====================================
// 事件产生（任务）
//=====================================
Event pollLedEvent() {
  static unsigned long last_time = 0;
  unsigned long now = millis();

  if (Serial.available()) {
    char c = Serial.read();
    if (c == 'f') {
      return Event::FORCE_OFF;
    }
  }
  if(now - last_time >= LED_INTERVAL_MS) {
    last_time = now;
    return Event::TIMEOUT;
  }
  return Event::NONE;
}

//=====================================
// 状态机引擎（通用）
//=====================================
void updateLedStateMachine(Event event){ 
  if(event == Event::NONE) {
    return; 
  }

  for(size_t i = 0; i < TRANSITION_COUNT; ++i){
    const Transition& t = transitions[i];

    if(t.current == led_state && t.event == event) {
      if(t.action){
        t.action();
      }
      led_state = t.next;
      break;
    }
  }
}

//=====================================
// Arduino生命周期
//=====================================
void setup() {
  Serial.begin(9600);
}
void loop() {
  Event event = pollLedEvent();
  updateLedStateMachine(event);
}