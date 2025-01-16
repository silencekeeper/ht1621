#include "esphome.h"

class HT1621DisplayComponent : public PollingComponent {
 public:
  HT1621DisplayComponent() : PollingComponent(1000) {}

  void setup() override {
    pinMode(CS_PIN, OUTPUT);
    pinMode(DATA_PIN, OUTPUT);
    pinMode(CLK_PIN, OUTPUT);
    digitalWrite(CS_PIN, HIGH);
  }

  void update() override {
    // Отображение температуры с DS18B20
    float temperature = id(temperature_sensor).state;
    char buffer[6];
    snprintf(buffer, sizeof(buffer), "%.1f", temperature); // Преобразование в строку
    displayText(buffer);
  }

 private:
  const int CS_PIN = 12;    // Chip Select
  const int DATA_PIN = 15; // Data
  const int CLK_PIN = 14;  // Clock
//D2 – SD_MISO
//D12 – SD_CS
//D14 – SD_SCLK
//D15 – SD_MOSI
  void sendCommand(uint8_t command) {
    digitalWrite(CS_PIN, LOW);
    sendBits(0b100, 3);   // Передача команды
    sendBits(command, 8); // Передача данных команды
    digitalWrite(CS_PIN, HIGH);
  }

  void sendData(uint8_t address, uint8_t data) {
    digitalWrite(CS_PIN, LOW);
    sendBits(0b101, 3);        // Передача данных
    sendBits(address << 3, 6); // Адрес (6 бит)
    sendBits(data, 8);         // Данные (8 бит)
    digitalWrite(CS_PIN, HIGH);
  }

  void sendBits(uint16_t data, uint8_t bits) {
    for (int i = bits - 1; i >= 0; i--) {
      digitalWrite(DATA_PIN, (data >> i) & 1);
      digitalWrite(CLK_PIN, HIGH);
      delayMicroseconds(1);
      digitalWrite(CLK_PIN, LOW);
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
      0b00111111, // 0
      0b00000110, // 1
      0b01011011, // 2
      0b01001111, // 3
      0b01100110, // 4
      0b01101101, // 5
      0b01111101, // 6
      0b00000111, // 7
      0b01111111, // 8
      0b01101111  // 9
    };

    if (c >= '0' && c <= '9') {
      return segment_map[c - '0'];
    } else if (c == '.') {
      return 0b10000000; // Точка
    } else {
      return 0; // Пробел или неизвестный символ
    }
  }
};