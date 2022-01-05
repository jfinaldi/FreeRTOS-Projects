// Use only core 1 for demo
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

// Pins
static const int led_pin = LED_BUILTIN;

// Our task: blink an LED on for 10 sec and off for 2 sec
void toggleLED(void* parameter) {
  while(1) {
    digitalWrite(led_pin, LOW);             // led light off
    vTaskDelay(2000 / portTICK_PERIOD_MS);  // delay for 2s
    digitalWrite(led_pin, HIGH);            // led light on
    vTaskDelay(10000 / portTICK_PERIOD_MS); // delay for 10000 ms or 10 sec
  }
}

void setup() {
  // configure led pin to be an output (instead of input)
  pinMode(led_pin, OUTPUT);

  // call the task create function; tell our task to run on only one core
  xTaskCreatePinnedToCore(  // Use xTaskCreate() in vanilla FreeRTOS
              toggleLED,    // Function to be called
              "Toggle LED", // Name of the task
              1024,         // Stack size (bytes in ESP32, words in FreeRTOS) (min for empty task is 768 bytes)
              NULL,         // Parameter to pss to function (next task)
              1,            // Task priority (0 to confixMAX_PRIORITIES - 1)
              NULL,         // Task handle
              app_cpu);     // Run on one core for demo purposes (ESP32 only)
              
  // If this was vanilla FreeRTOS, you'd want to call vTaskStartScheduler() in
  // main after setting up your tasks.
}

void loop() {
  // put your main code here, to run repeatedly:
}
