#define task_list_size 256
#define task_delay_list_size 16

volatile uint8_t kernel_delay_closest_time_index;
volatile uint32_t kernel_delay_closest_time;
volatile uint8_t kernel_delay_current_index;

volatile uint32_t kernel_delay_times[task_delay_list_size];
volatile uint16_t kernel_delay_addresses[task_delay_list_size];

volatile uint32_t kernel_time_ms;
volatile uint16_t kernel_task_list[task_list_size];
volatile uint8_t kernel_current_task_index;
volatile uint8_t kernel_last_task_index;

void kernel_blackhole() {
  cli();
  kernel_task_list[kernel_last_task_index] = kernel_blackhole;
  kernel_last_task_index++;
  sei();
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
void kernel_add_task(uint16_t address) {
  kernel_task_list[kernel_last_task_index] = address;
  kernel_last_task_index++;
}
uint8_t kernel_delay(uint16_t duration, uint16_t addr) {
  uint8_t current_index = 0;
  while (current_index != task_delay_list_size) {
    if (kernel_delay_addresses[current_index] == 0) {
      break;
    }
    current_index++;
  }
  if (current_index != task_delay_list_size) {
    kernel_delay_addresses[current_index] = addr;
    kernel_delay_times[current_index] = duration + kernel_time_ms;
    return 0;
  }
  return 1;
}
void kernel_delay_closest_finderB() {
  if (kernel_delay_current_index != task_delay_list_size) {
    if (kernel_delay_addresses[kernel_delay_current_index] != 0) {
      if (kernel_delay_closest_time > kernel_delay_times[kernel_delay_current_index]) {
        kernel_delay_closest_time_index = kernel_delay_current_index;
        kernel_delay_closest_time = kernel_delay_times[kernel_delay_current_index];
      }
    }
    cli();
    kernel_task_list[kernel_last_task_index] = kernel_delay_closest_finderB;
    kernel_last_task_index++;
    sei();
    kernel_delay_current_index++;
  } else {
    cli();
    TIFR2 |= (1 << OCF2B);
    TIMSK2 |= (1 << OCIE2B);
    sei();
  }
}
void kernel_delay_closest_finderA() {
  kernel_delay_closest_time = -1;
  kernel_delay_current_index = 0;
  cli();
  kernel_task_list[kernel_last_task_index] = kernel_delay_closest_finderB;
  kernel_last_task_index++;
  sei();
}

ISR(TIMER2_COMPA_vect) {
  kernel_time_ms++;
}
ISR(TIMER2_COMPB_vect) {
  if (kernel_delay_addresses[kernel_delay_closest_time_index] != 0) {
    if (kernel_time_ms >= kernel_delay_times[kernel_delay_closest_time_index]) {
      kernel_task_list[kernel_last_task_index] = kernel_delay_addresses[kernel_delay_closest_time_index];
      kernel_last_task_index++;
      kernel_task_list[kernel_last_task_index] = kernel_delay_closest_finderA;
      kernel_last_task_index++;
      kernel_delay_addresses[kernel_delay_closest_time_index] = 0;
      kernel_delay_times[kernel_delay_closest_time_index] = 0;
      kernel_delay_closest_time_index = 0;
      TIMSK2 &= ~(1 << OCIE2B);
    }
  }
}
