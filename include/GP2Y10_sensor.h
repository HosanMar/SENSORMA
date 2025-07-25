#ifndef GP2Y10_SENSOR_H
#define GP2Y10_SENSOR_H

#include <Arduino.h>

#define GP2Y10_LED_PIN 2        // Pino digital que controla o LED do sensor
#define GP2Y10_MEASURE_PIN 33   // Pino analógico conectado ao Vo (saída analógica do sensor)

void initGP2Y10() {
  pinMode(GP2Y10_LED_PIN, OUTPUT);
  digitalWrite(GP2Y10_LED_PIN, LOW);
}

float readGP2Y10() {
  digitalWrite(GP2Y10_LED_PIN, HIGH);     // Liga o LED IR
  delayMicroseconds(280);                 // Tempo de amostragem
  int raw = analogRead(GP2Y10_MEASURE_PIN); // Leitura analógica
  digitalWrite(GP2Y10_LED_PIN, LOW);      // Desliga o LED

  float voltage = raw * (3.3 / 4095.0);   // Conversão para volts (ESP32 ADC 12-bit)
  float dustDensity = (voltage - 0.4) / 0.05; // Fórmula típica para µg/m³

  // Limita resultado mínimo a zero
  return dustDensity < 0 ? 0 : dustDensity;
}

#endif  // GP2Y10_SENSOR_H
