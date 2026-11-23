#ifndef MQ7_SENSOR_H
#define MQ7_SENSOR_H

#include <Arduino.h>
#include "config.h"
#define MQ7_PIN 35
#define RL_MQ7 10000.0        // Resistência de carga (ohms) na placa do sensor
#define R0_MQ7 (29000 / 27.0)       // R0 calibrado em ar limpo (ohms)
void initMQ7() {
  pinMode(MQ7_PIN, INPUT);
}

float readMQ7() {
  // 1. Suavização (Média de 50 leituras para estabilizar o sinal)
  long sum = 0;
  for (int i = 0; i < 50; i++) {
    sum += analogRead(MQ7_PIN);
    delay(5); // Pequena pausa para estabilização do ADC
  }
  float avgAnalogValue = sum / 50.0;
  
  // 2. Tensão lida pelo ESP32 (atenuada pelo divisor de tensão)
  float Vout_ESP = avgAnalogValue * (V_ADC / 4095.0); 

  if (Vout_ESP <= 0.0) return 0.0; // Evita divisão por zero

  // 3. Reversão matemática do Divisor de Tensão (R1=10k, R2=15k)
  // V_ESP = V_Sensor * (15 / (10 + 15)) -> V_ESP = V_Sensor * 0.6
  float Vout_Sensor = Vout_ESP / 0.6;

  // 4. Cálculo de Rs e do Ratio usando a tensão real do sensor
  float Rs = RL_MQ7 * ((V_SENSOR - Vout_Sensor) / Vout_Sensor);
  float ratio = Rs / R0_MQ7;

  // Equação aproximada do datasheet MQ-7
  float ppm = pow(10, (-1.497 * log10(ratio) + 0.518));

  // Conversão de ppm para mg/m³ (Monóxido de Carbono - CO)
  float mgm3 = ppm * 1.146;

  Serial.print(" MQ7 - Rs: ");
  Serial.print(Rs);
  Serial.print(" | Ratio: ");
  Serial.print(ratio);
  Serial.print(" | ppm: ");
  Serial.println(ppm);
  
  return mgm3;
} 
#endif
