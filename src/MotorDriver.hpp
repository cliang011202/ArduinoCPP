#pragma once
#include <Arduino.h>

class MotorDriver {
  public:
    explicit MotorDriver(uint8_t pin)
    :pin_(pin) {
      pinMode(pin_,OUTPUT);
      stop();
    }

    void start() {
      digitalWrite(pin_,HIGH);
    }

    void stop() {
      digitalWrite(pin_,LOW);
    }

  private:
    const uint8_t pin_;
};