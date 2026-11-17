#ifndef GP2Y10_SENSOR_H
#define GP2Y10_SENSOR_H

#include <Arduino.h>

#define GP2Y10_LED_PIN 2        // Pino digital que controla o LED do sensor
#define GP2Y10_MEASURE_PIN 32   // Pino analógico conectado ao Vo (saída analógica do sensor)
#define V_ADC 3.3             // Tensão de referência do ADC da ESP32

void initGP2Y10() {
  pinMode(GP2Y10_LED_PIN, OUTPUT);
  digitalWrite(GP2Y10_LED_PIN, LOW);
}

float readGP2Y10() {
  digitalWrite(GP2Y10_LED_PIN, HIGH);     // Liga o LED IR
  delayMicroseconds(280);                 // Tempo de amostragem

  int raw = analogRead(GP2Y10_MEASURE_PIN); // Leitura analógica
  delayMicroseconds(40);                    // Completa o pulso de 320µs
  digitalWrite(GP2Y10_LED_PIN, LOW);      // Desliga o LED
  delayMicroseconds(9680);                  // Repouso para completar o ciclo de 10ms


  // 1. Tensão lida pelo ESP32 (atenuada pelo divisor de tensão)
  float Vout_ESP = raw * (V_ADC / 4095.0);

  // 2. Reversão matemática do Divisor de Tensão (R1=10k, R2=15k)
  // V_ESP = V_Sensor * (15 / (10 + 15)) -> V_ESP = V_Sensor * 0.6
  float Vout_Sensor = Vout_ESP / 0.6;

  // 3. Fórmula de conversão típica (tensão real para densidade)
  float dustDensity = (Vout_Sensor - 0.4) / 0.05; 

  // Limita o resultado mínimo a zero 
  return dustDensity < 0.0 ? 0.0 : dustDensity;
}

#endif  // GP2Y10_SENSOR_H
