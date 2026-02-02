#pragma once

class PeriodicTask{
public:
  PeriodicTask(unsigned long interval)
    : interval_(interval){}

  bool expired() {
    unsigned long now = millis();
    if(now- last_time_ >= interval_) {
      last_time_ = now;
      return true;
    }
    return false;
  }

private:
  unsigned long last_time_ = 0;
  const unsigned long interval_;
};