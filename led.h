uint16_t led_task_adresses[3];
uint16_t led_task_delay_duration = 0;
uint16_t led_task_delay_next_addr = kernel_blackhole;
uint32_t led_task_delay_start_time = 0;
uint8_t led_blink_count;

void led_on() {
  led_blink_count++;
  digitalWrite(13, 1);
  kernel_delay(200, led_task_adresses[1]);
  /*
    led_task_delay_next_addr = led_task_adresses[1];
    led_task_delay_start_time = millis();
    kernel_add_task(led_task_adresses[2]);
  */
}

void led_off() {
  digitalWrite(13, 0);
  kernel_delay(200, led_task_adresses[2]);
  /*
    led_task_delay_next_addr = led_task_adresses[0];
    led_task_delay_start_time = millis();
    kernel_add_task(led_task_adresses[2]);
  */
}

void led_kill() {
  if (led_blink_count == 10) {
    digitalWrite(13, 0);
    pinMode(13, INPUT);
  } else {
    kernel_add_task(led_task_adresses[0]);
  }
}

void led_init() {
  led_task_adresses[0] = led_on;
  led_task_adresses[1] = led_off;
  led_task_adresses[2] = led_kill;

  pinMode(13, OUTPUT);
  digitalWrite(13, 0);
  led_blink_count = 0;
  led_task_delay_duration = 200;
  kernel_add_task(led_task_adresses[0]);
}
