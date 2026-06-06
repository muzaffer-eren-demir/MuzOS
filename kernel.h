#define task_list_size 256
uint16_t kernel_task_list[task_list_size];
uint8_t kernel_current_task_index;
uint8_t kernel_last_task_index;

void kernel_blackhole() {
  Serial.println("BlackHole");
  while (1);
}

void kernel_init() {
  uint16_t a = 0;
  while (a != task_list_size) {
    kernel_task_list[a] = kernel_blackhole;
    a++;
  }
  kernel_current_task_index = 0;
  kernel_last_task_index = 0;
}

void kernel_scheduler() {
  ((void(*)())kernel_task_list[kernel_current_task_index])();
  kernel_task_list[kernel_current_task_index] = kernel_blackhole;
  kernel_current_task_index++;
}
void kernel_add_task(uint16_t address) {
  kernel_task_list[kernel_last_task_index] = address;
  kernel_last_task_index++;
}
