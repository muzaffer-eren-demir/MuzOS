#include "kernel.h"
#include "led.h"
#include "serial.h"
void setup() {
  Serial.begin(230400);
  Serial.println("Started");
  kernel_init();
  kernel_add_task(led_init);
  kernel_add_task(serial_init);
}

void loop() {
  kernel_scheduler();
  Serial.print("current: ");
  Serial.print(kernel_current_task_index);
  Serial.print(", last: ");
  Serial.print(kernel_last_task_index);
  Serial.print(", diff: ");
  Serial.println(uint8_t(kernel_last_task_index - kernel_current_task_index));
}
