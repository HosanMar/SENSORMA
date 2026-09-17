#ifndef MQ7_SENSOR_H
#define MQ7_SENSOR_H

#include <Arduino.h>

#define MQ7_PIN 32
#define RL 10000.0        // Resistência de carga (ohms) na placa do sensor
#define V_ADC 3.3         // Tensão máxima de leitura do ADC da ESP32
#define V_SENSOR 5.0      // Tensão de alimentação real do sensor (VCC)
#define R0 4221.7         // Valor temporário (vamos recalibrar abaixo)

void initMQ7() {
  pinMode(MQ7_PIN, INPUT);
}

float readMQ7() {
  // 1. Suavização (Média de 50 leituras para estabilizar o sinal)
  long sum = 0;
  for (int i = 0; i < 50; i++) {
    sum += analogRead(MQ7_PIN);
    delay(5); // Pequena pausa para o ADC da ESP32 respirar
  }
  float avgAnalogValue = sum / 50.0;
  
  // 2. Tensão direta que chega no pino (sem resistores externos)
  float Vout = avgAnalogValue * (V_ADC / 4095.0); 

  if (Vout == 0) return 0.0; // Evita divisão por zero

  // 3. Cálculo do Rs baseado na tensão direta do pino
  float Rs = RL * ((V_SENSOR - Vout) / Vout);
  float ratio = Rs / R0;

  // Equação aproximada do datasheet MQ-7
  float ppm = pow(10, (-1.497 * log10(ratio) + 0.518));

  // Conversão ppm → mg/m³ para o monóxido de carbono (CO)
  float mgm3 = ppm * 1.146;
  
  Serial.print("Rs: ");
  Serial.print(Rs);
  Serial.print(" | Ratio: ");
  Serial.print(ratio);
  Serial.print(" | ppm: ");
  Serial.println(ppm);

  return mgm3;
}

#endif
