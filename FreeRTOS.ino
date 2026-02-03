#include <Arduino_FreeRTOS.h>
#include <queue.h>
#include <semphr.h>
#include <event_groups.h>
#include "MotorStateMachine.hpp"

struct LogMessage {
  char text[32];
};

MotorDriver motor(13);
MotorStateMachine motorSM(motor);
QueueHandle_t logQueue;
QueueHandle_t motorEventQueue;
SemaphoreHandle_t serialMutex;

#define EVENT_MOTOR_ON (1 << 0)
#define EVENT_MOTOR_OFF (1 << 1)

void MotorControlTask(void *pv) {
  MotorEvent event;
  LogMessage log;

  const TickType_t period = pdMS_TO_TICKS(10);//10 / portTICK_PERIOD_MS;
  TickType_t lastWake = xTaskGetTickCount();

  while (1) {
    if(xQueueReceive(motorEventQueue,&event,portMAX_DELAY)){
      motorSM.handleEvent(event);
    }

    vTaskDelayUntil(&lastWake,period);
  }
}

void CommandTask(void *pv) {
  char cmd;
  LogMessage log;
  while (1) {
    if (Serial.available()) {
      if (xSemaphoreTake(serialMutex, portMAX_DELAY)) {
        cmd = Serial.read();
        xSemaphoreGive(serialMutex);
      
        if(cmd == '1'){
          MotorEvent event = MotorEvent::CMD_START;
          xQueueSend(motorEventQueue,&event,0);
          strcpy(log.text,"motor start");
          xQueueSend(logQueue,&log,portMAX_DELAY);
        }else if(cmd == '0'){
          MotorEvent event = MotorEvent::CMD_STOP;
          xQueueSend(motorEventQueue,&event,0);
          strcpy(log.text,"motor stop");
          xQueueSend(logQueue,&log,portMAX_DELAY);
        }
      }
    }
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

void LoggerTask(void *pv) {
  LogMessage log;
  while (1) {
    if (xQueueReceive(logQueue, &log, portMAX_DELAY)) {
      if (xSemaphoreTake(serialMutex, portMAX_DELAY)) {
        Serial.println(log.text);
        xSemaphoreGive(serialMutex);
      }
    }
  }
}

void setup() {
  Serial.begin(9600);

  logQueue = xQueueCreate(5, sizeof(LogMessage));
  motorEventQueue = xQueueCreate(5,sizeof(MotorEvent));
  serialMutex = xSemaphoreCreateMutex();  

  xTaskCreate(MotorControlTask, "Motor", 256, NULL, 3, NULL);
  xTaskCreate(CommandTask, "Cmd", 128, NULL, 2, NULL);
  xTaskCreate(LoggerTask, "Log", 128, NULL, 1, NULL);
}

void loop() {}