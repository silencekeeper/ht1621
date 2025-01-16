#pragma once

#include "esphome.h"

class HT1621Display : public PollingComponent, public display::DisplayBuffer {
 public:
  HT1621Display() : PollingComponent(1000) {}

  void setup() override {
    pinMode(CS_PIN, OUTPUT);
    pinMode(DATA_PIN, OUTPUT);
    pinMode(WR_PIN, OUTPUT);
    digitalWrite(CS_PIN, HIGH);  // Деактивируем CS
  }

  void update() override {
    // Обновление экрана (например, вывод температуры)
    float temperature = id(temperature_sensor).state;
    char buffer[6];
    snprintf(buffer, sizeof(buffer), "%.1f", temperature);
    displayText(buffer);
  }

  void draw_absolute_pixel_internal(int x, int y, Color color) override {
    // Не требуется для семисегментного дисплея
  }

 private:
  const int CS_PIN = 16;    // Chip Select
  const int DATA_PIN = 32; // Data
  const int CLK_PIN = 4;  // Clock

  void sendCommand(uint8_t command) {
    digitalWrite(CS_PIN, LOW);
    sendBits(0b100, 3);
    sendBits(command, 8);
    digitalWrite(CS_PIN, HIGH);
  }

  void sendData(uint8_t address, uint8_t data) {
    digitalWrite(CS_PIN, LOW);
    sendBits(0b101, 3);
    sendBits(address << 3, 6);
    sendBits(data, 8);
    digitalWrite(CS_PIN, HIGH);
  }

  void sendBits(uint16_t data, uint8_t bits) {
    for (int i = bits - 1; i >= 0; i--) {
      digitalWrite(DATA_PIN, (data >> i) & 1);
      digitalWrite(WR_PIN, HIGH);
      delayMicroseconds(1);
      digitalWrite(WR_PIN, LOW);
    }
  }

  void displayText(const char *text) {
    for (int i = 0; i < 5; i++) {
      uint8_t segments = encodeCharacter(text[i]);
      sendData(i, segments);
    }
  }

  uint8_t encodeCharacter(char c) {
    static const uint8_t segment_map[10] = {
        0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110,
        0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01101111};

    if (c >= '0' && c <= '9') {
      return segment_map[c - '0'];
    } else if (c == '.') {
      return 0b10000000;
    } else {
      return 0;
    }
  }
};