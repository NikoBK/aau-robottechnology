/* 
  file: exercise1.ino
  author: nikobk
  created on: november 2, 2023
*/

/* <nikobk>:
  The ESP32-C3-DevKitM-1 comes with FreeRTOS preinstalled (a modified version).
  This modified version contains a Arduino library that allows you to write Arduino
  code to the ESP32-C3's single core CPU.
  C3 doc: https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/hw-reference/esp32c3/user-guide-devkitm-1.html

  There is no built-in LED other than the 5 voltage power on/off LED, and a RGB LED, it is important to note
  that the RGB LED is controlled over CPIO8 which is located on the input/output pin 8.
  I use the library: "Freenove WS2812 Lib for ESP32" to learn how to use this RGB LED.
  Once installed there should be a "RGBW" example uner the Freenove library in the examples dropdown.
  
  NOTE: This is an earlier commit version of adem-esp32-rtos.ino
*/

// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

int delayMS = 500;
bool verbose = false;

// Arbitrary number for exercise 1.
int value = 10;

// Create a new task.
void incrementTask(void *parameter) {
  // Our superloop.
  while(1) {
    // Use vTaskDelay instead of delay() as it is nonblocking and works on freeRTOS.
    // portTICK refers to one of the hardware ticktimers (clocks).
    Serial.println("Current value is: " + String(value));
    value += 10;
    vTaskDelay(delayMS / portTICK_PERIOD_MS);
    if (verbose) {
      Serial.println("<Task1>: Tick!");
    }
  }
}

void setup() {
  // I found this frequency to work on the ESP32 C3.
  Serial.begin(115200);

  // Make sure our task only runs on one of the cores.
  xTaskCreatePinnedToCore(
        incrementTask,       // Function to be called for the task
        "Number Increment 1",       // Task Name
        1024,              // Stack size - 1kb (bytes in ESP32, words in vanilla freeRTOS)
        NULL,              // Parameter to pass to function
        1,                 // Task prioerity (0 to configMAX_PRIORITIES - 1)
        NULL,              // Task handle
        app_cpu);          // Run on one core for demo purposes (ESP32 only)
}

void loop() {
  // put your main code here, to run repeatedly:
}