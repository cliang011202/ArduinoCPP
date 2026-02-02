#include <Arduino_FreeRTOS.h>
#include <queue.h>
#include <semphr.h>
#include <event_groups.h>

constexpr int motor1Pin = 9;

struct LogMessage {
  char text[32];
};

struct MotorTarget {
  bool enable;
  uint8_t duty; //0~255
};

MotorTarget motorTarget;
QueueHandle_t logQueue;
SemaphoreHandle_t serialMutex;
SemaphoreHandle_t targetMutex;
EventGroupHandle_t systemEvent;

#define EVENT_MOTOR_ON (1 << 0)
#define EVENT_MOTOR_OFF (1 << 1)

void MotorControlTask(void *pv) {
  const TickType_t period = 10 / portTICK_PERIOD_MS;
  TickType_t lastWake = xTaskGetTickCount();

  pinMode(motor1Pin, OUTPUT);
  LogMessage log;

  while (1) {
    MotorTarget localTarget;
    EventBits_t bits = xEventGroupWaitBits(
      systemEvent,
      EVENT_MOTOR_ON | EVENT_MOTOR_OFF,
      pdTRUE,   //自动删除
      pdFALSE,  //任意一个即可
      portMAX_DELAY
    );

    if(bits & EVENT_MOTOR_ON){
      strcpy(log.text,"Motor ON");
      xQueueSend(logQueue,&log,portMAX_DELAY);
    }

    if(bits & EVENT_MOTOR_OFF){
      strcpy(log.text,"Motor OFF");
      xQueueSend(logQueue,&log,portMAX_DELAY);
    }

    if(localTarget.enable){
      analogWrite(motor1Pin,localTarget.duty);
    }else{
      analogWrite(motor1Pin,0);
    }

    vTaskDelayUntil(&lastWake,period);
  }
}

void CommandTask(void *pv) {
  char cmd;
  while (1) {
    if (Serial.available()) {
      if (xSemaphoreTake(serialMutex, portMAX_DELAY)) {
        cmd = Serial.read();
        xSemaphoreGive(serialMutex);

        if(xSemaphoreTake(targetMutex,portMAX_DELAY)){
          if(cmd == '1'){
            motorTarget.enable = true;
            xEventGroupSetBits(systemEvent,EVENT_MOTOR_ON);
          }else if(cmd == '0'){
            motorTarget.enable = false;
            xEventGroupSetBits(systemEvent,EVENT_MOTOR_OFF);
          }else if(cmd >= '0' && cmd<='9'){
            motorTarget.duty = (cmd - '0') * 25;
          }
          xSemaphoreGive(targetMutex);
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
  serialMutex = xSemaphoreCreateMutex();
  targetMutex = xSemaphoreCreateMutex();
  systemEvent = xEventGroupCreate();

  xTaskCreate(MotorControlTask, "Motor", 128, NULL, 3, NULL);
  xTaskCreate(CommandTask, "Cmd", 128, NULL, 2, NULL);
  xTaskCreate(LoggerTask, "Log", 128, NULL, 1, NULL);
}

void loop() {}