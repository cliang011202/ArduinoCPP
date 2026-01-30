#include "Led.hpp"
// #include "EventQueue.hpp"

//=====================================
// 常量定义
//=====================================
constexpr uint8_t LED_PIN = 13;
constexpr unsigned long LED_INTERVAL_MS = 5000;
constexpr size_t EVENT_QUEUE_SIZE = 8;

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

class EventQueue {
public:
  bool push(Event e) {
    if (count_ >= EVENT_QUEUE_SIZE) {
      if(e == Event::TIMEOUT){
        return false;//队列满
      }
      head_ = (head_ + 1) % EVENT_QUEUE_SIZE;
      --count_;
      
    }
    buffer_[tail_] = e;
    tail_ = (tail_ + 1) % EVENT_QUEUE_SIZE;
    ++count_;
    return true;
  }

  bool pop(Event& e) {
    if(count_ == 0){
      return false; //队列空
    }
    e = buffer_[head_];
    head_ = (head_ + 1) % EVENT_QUEUE_SIZE;
    --count_;
    return true;
  }

private:
  Event buffer_[EVENT_QUEUE_SIZE];
  size_t head_ = 0;
  size_t tail_ = 0;
  size_t count_ = 0;
};

//=====================================
// 全局对象&状态
//=====================================
Led led(LED_PIN);
LedState led_state = LedState::OFF;
EventQueue event_queue;

//=====================================
// 动作函数（Action）
//=====================================
void actionLedOn() {
  led.on();
}
void actionLedOff() {
  led.off();
}
void taskPollEvent() {
  static unsigned long last_time = 0;
  unsigned long now = millis();

  if(now - last_time >= LED_INTERVAL_MS) {
    last_time = now;
    event_queue.push(Event::TIMEOUT);
  }

  if(Serial.available()) {
    char c = Serial.read();
    if(c == 'f') {
      event_queue.push(Event::FORCE_OFF);
    }
  }
}

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
void updateStateMachine(Event event){ 
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

void taskStateMachine() {
  Event event;
  while (event_queue.pop(event)) {
    updateStateMachine(event);
  }
}

//=====================================
// Arduino生命周期
//=====================================
void setup() {
  Serial.begin(9600);
}
void loop() {
  taskPollEvent();
  taskStateMachine();
}