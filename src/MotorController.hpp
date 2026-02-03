#pragma once
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

enum class MotorCmdType {
  SET_TARGET,
  STOP
};

struct MotorTarget{
  bool enable;
  uint8_t duty;
};

struct MotorCommand{
  MotorCmdType type;
  uint8_t duty;
};

class MotorController {
public:
  explicit MotorController(uint8_t pwmPin);
  void start();
  void setTarget(uint8_t duty);
  void stop();

private:
  static void taskEntry(void *arg);
  void taskLoop();

  uint8_t pin_;
  MotorTarget current_;
  QueueHandle_t cmdQueue_;
  // SemaphoreHandle_t mutex_;
  TaskHandle_t taskHandle_;
};