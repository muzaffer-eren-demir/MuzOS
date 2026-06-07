#include "kernel.h"
#include "led.h"
#include "serial.h"
void setup() {
  Serial.begin(230400);
  Serial.println("Started");
  uint16_t a = 0;
  while (a != task_list_size) {
    kernel_task_list[a] = kernel_blackhole;
    a++;
  }

  kernel_current_task_index = 0;
  kernel_last_task_index = 0;

  cli();
  TCCR2A = 0;
  TCNT2  = 0;
  OCR2A = 249;
  OCR2B = 249;
  TCCR2A |= (1 << WGM21);
  TCCR2B |= (1 << CS22);
  TIMSK2 |= (1 << OCIE2A) | (1 << OCIE2B);
  sei();
  kernel_time_ms = 0;
  kernel_delay_closest_time_index = 0;

  kernel_add_task(led_init);
  kernel_add_task(serial_init);
}

void loop() {
  ((void(*)())kernel_task_list[kernel_current_task_index])();
  cli();
  kernel_task_list[kernel_current_task_index] = kernel_blackhole;
  kernel_current_task_index++;
  sei();
  Serial.print("current: ");
  Serial.print(kernel_current_task_index);
  Serial.print(", last: ");
  Serial.print(kernel_last_task_index);
  Serial.print(", diff: ");
  Serial.print(uint8_t(kernel_last_task_index - kernel_current_task_index));
  Serial.print(", time: ");
  Serial.println(kernel_time_ms);

}
