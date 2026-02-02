#include "MotorController.hpp"

MotorController::MotorController(uint8_t pwmPin)
:pin_(pwmPin) {
  current_.enable = false;
  current_.duty = 0;
  cmdQueue_ = xQueueCreate(5,sizeof(MotorCommand));
}

void MotorController::start(){
  pinMode(pin_,OUTPUT);
  xTaskCreate(
    taskEntry,
    "taskEntry",
    256,
    this,
    3,
    &taskHandle_
  );
}

void MotorController::setTarget(uint8_t duty) {
  MotorCommand cmd{
    MotorCmdType::SET_TARGET,
    duty
  };
  xQueueSend(cmdQueue_,&cmd,0);
}

void MotorController::stop(){
  MotorCommand cmd{
    MotorCmdType::STOP,
    0
  };
  xQueueSend(cmdQueue_,&cmd,0);
}

void MotorController::taskEntry(void *arg) {
  static_cast<MotorController*>(arg)->taskLoop();
}

void MotorController::taskLoop(){
  const TickType_t period = 10 / portTICK_PERIOD_MS;
  TickTyper_t lastWake = xTaskGetTickCount();
  MotorCommand cmd;

  while(1) {
    while(xQueueReceive(cmdQueue_,&cmd,0)){
      if(cmd.type == MotorCmdType::SET_TARGET){
        current_.enable = true;
        current_.duty = cmd.duty;
      } else if(cmd.type == MotorCmdType::STOP) {
        current_.enable = false;
      }
    }

    if(current_.enable) {
      analogWrite(pin_,current_.duty);
    } else {
      analogWrite(pin_,0);
    }

    vTaskDelayUntil(&lastWake,period);
  }
}