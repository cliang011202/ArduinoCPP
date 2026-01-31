#include <Arduino_FreeRTOS.h>

void TaskBlink(void *pvParameters){
  pinMode(13,OUTPUT);

  while(1){
    digitalWrite(13,HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    digitalWrite(13,LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void TaskPrint(void *pvParameters){
  Serial.begin(9600);

  while(1){
    Serial.println("TaskPrint running...");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void setup()
{
  xTaskCreate(TaskBlink,"Blink",128,NULL,2,NULL);
  xTaskCreate(TaskPrint,"Print",128,NULL,2,NULL);
}

void loop()
{

}