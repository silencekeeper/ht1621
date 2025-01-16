#include "ht1621_display.h"

void HT1621Display::update() {
  // Обновление дисплея (пример: вывод числа)
  float temperature = id(temperature_sensor).state;
  char buffer[6];
  snprintf(buffer, sizeof(buffer), "%.1f", temperature);
  displayText(buffer);
}