#include <Arduino.h>
#ifndef MQ135_SENSOR_H
#define MQ135_SENSOR_H

#include "config.h"

#define MQ135_PIN 34     // Utiliza GPIO34 para não conflitar com o MQ-7
#define RL_MQ135 10000.0 // Resistência de carga RL (ohms) na placa do sensor
#define R0_MQ135 37500 // R0 calibrado em ar limpo (ohms)

void initMQ135() {
  pinMode(MQ135_PIN, INPUT);
}

float readMQ135() {
  // 1. Suavização (Média de 50 leituras para estabilizar o sinal)
  long sum = 0;
  for (int i = 0; i < 50; i++) {
    sum += analogRead(MQ135_PIN);
    delay(5);
  }
  float avgAnalogValue = sum / 50.0;

  // 2. Tensão lida pelo ESP32
  float Vout_ESP = avgAnalogValue * (V_ADC / 4095.0);

  if (Vout_ESP <= 0.0) return 0.0; // Evita divisão por zero

  // 3. Reversão matemática do Divisor de Tensão (R1=10k, R2=15k)
  // V_ESP = V_Sensor * (15 / (10 + 15)) -> V_ESP = V_Sensor * 0.6
  float Vout_Sensor = Vout_ESP / 0.6;

  // 4. Cálculo de Rs e do Ratio usando a tensão real do sensor
  float Rs = RL_MQ135 * ((V_SENSOR - Vout_Sensor) / Vout_Sensor);
  float ratio = Rs / R0_MQ135;

  // 5. Curva do datasheet (Aproximação logarítmica)
  // ppm = 10 ^ (a * log10(ratio) + b)
  float a = -2.381;  // Inclinação angular extraída do gráfico
  float b = 0.857;   // Constante de intercepto

  float ppm = pow(10, (a * log10(ratio) + b));

  Serial.print(" MQ135 - Rs: ");
  Serial.print(Rs);
  Serial.print(" | Ratio: ");
  Serial.print(ratio);
  Serial.print(" | ppm: ");
  Serial.println(ppm);

  return ppm;
}

#endif
