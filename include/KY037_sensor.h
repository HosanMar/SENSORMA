#ifndef KY037_SENSOR_H
#define KY037_SENSOR_H

#include <Arduino.h>

#define KY037_PIN 39 // Pino ADC (VN - Input Only) conectado ao A0 do sensor
const int sampleWindow = 50; // Largura da janela de amostragem (50 ms)


void initKY037() {
  pinMode(KY037_PIN, INPUT);
  analogSetAttenuation(ADC_11db); // Ajusta ADC para ~3.3V
}

float readKY037() {
  unsigned long startMillis = millis();
  int peakToPeak = 0;
  unsigned int signalMax = 0;
  unsigned int signalMin = 4095;

  // Realiza leituras contínuas durante a janela de 50 milissegundos
  while (millis() - startMillis < sampleWindow) {
    int sample = analogRead(KY037_PIN);

    // Filtra eventuais ruídos fora da escala do ADC (12 bits)
    if (sample < 4095) {
      if (sample > signalMax) {
        signalMax = sample;  // Salva o pico máximo da onda sonora
      } else if (sample < signalMin) {
        signalMin = sample;  // Salva o vale mínimo da onda sonora
      }
    }
  }

  // Calcula a amplitude pico a pico
  peakToPeak = signalMax - signalMin;

  // Converte a amplitude da resolução do ADC para porcentagem (0% a 100%)
  float noisePercentage = (peakToPeak / 4095.0) * 100.0;

  // Trava de segurança para limites
  if (noisePercentage > 100.0) noisePercentage = 100.0;
  if (noisePercentage < 0.0) noisePercentage = 0.0;

  return noisePercentage;
}

#endif
