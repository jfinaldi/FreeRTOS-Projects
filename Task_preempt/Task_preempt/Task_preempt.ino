/*
  Task Preemption Demo FreeRTOS

  Toggle LED and print "Hello, world" to the console,
  using separate tasks. 
*/

// Using only one core for this demo
#if CONFIG_REERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

// a string to print to the console
const char label[] = "Hello, my name is Jenn.\n";

// Taskhandles (these allow us to control these tasks from a third task
static TaskHandle_t task_1 = NULL;
static TaskHandle_t task_2 = NULL;

//**********************************************************************
// Tasks

// Task 1: print to serial terminal with lower priority
void startTask1(void* parameter) {
  // count the chars in the string
  int label_len = strlen(label);

  // print string
  while(1) {
    Serial.println(); // new line
    for(int i = 0; i < label_len; ++i) {
      Serial.print(label[i]);
    }
    Serial.println(); // new line
    vTaskDelay(1000 / portTICK_PERIOD_MS); // delay for 1 second
  }
}

// Task 2: print to Serial Terminal with higher priority
void startTask2(void* parameter) {
  // print asterisk every tenth of a second
  while(1) {
    Serial.println('*');
    vTaskDelay(100 / portTICK_PERIOD_MS); // delay for 1/10th a second
  }
}

//********************************************************************/

void setup() {
  // Configure Serial (we will go slow so we can see the preemption)
  Serial.begin(300); // 300 is the baud rate aka ticks per sec

  // Wait a moment to start (so we don't miss Serial output)
  vTaskDelay(1000 / portTICK_PERIOD_MS); // delay to give serial a chance to connect
  Serial.println();
  Serial.println("---FreeRTOS Task Demo---");

  // Print self priority
  Serial.print("Setup and loop task running on core ");
  Serial.print(xPortGetCoreID());
  Serial.print(" with priority ");
  Serial.println(uxTaskPriorityGet(NULL));

  // Task 1 to run forever
  xTaskCreatePinnedToCore(startTask1,
                          "Task 1",
                          1024,          // stack size
                          NULL,
                          1,             // priority 1 (low)
                          &task_1,
                          app_cpu);

  // Task 2 to run once with higher priority
  xTaskCreatePinnedToCore(startTask2,
                          "Task 2",
                          1024,          // stack size
                          NULL,
                          2,             // priority 2 (high)
                          &task_2,
                          app_cpu);
                          
}

void loop() {
  
  // Suspend the higher priority task for some intervals
  for(int i = 0; i < 3; ++i) {
    vTaskSuspend(task_2);                  // suspend task 2
    vTaskDelay(2000 / portTICK_PERIOD_MS); // delay 2 seconds
    vTaskResume(task_2);                   // resume task 2
    vTaskDelay(2000 / portTICK_PERIOD_MS); // delay 2 seconds 
  }

  // Delete the lower priority task
  if(task_1 != NULL) {

    // we never want to call this on a task that is already NULL
    // May cause memory overwritten or cause the processor to
    // crash and reset
    vTaskDelete(task_1); 
    task_1 = NULL;
  }

}
