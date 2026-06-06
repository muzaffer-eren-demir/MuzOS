uint16_t serial_task_adresses[2];

void serial_kontrol() {
  if (Serial.available()) {
    kernel_add_task(led_init);
    kernel_add_task(serial_task_adresses[1]);
  } else {
    kernel_add_task(serial_task_adresses[0]);
  }
}
void serial_temizle() {
  Serial.read();
  if (Serial.available()) {
    kernel_add_task(serial_task_adresses[1]);
  } else {
    kernel_add_task(serial_task_adresses[0]);
  }
}
void serial_init() {
  serial_task_adresses[0] = serial_kontrol;
  serial_task_adresses[1] = serial_temizle;
  kernel_add_task(serial_task_adresses[0]);
}
