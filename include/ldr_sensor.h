#ifndef LDR_SENSOR_H
#define LDR_SENSOR_H

#include <Arduino.h>
#define light_sensor_pin 36

void initLDR() {
  pinMode(light_sensor_pin, INPUT);
  analogSetAttenuation(ADC_11db);
}

int readLDR() {
  long sum = 0;
  for (int i = 0; i < 50; i++) {
    sum += analogRead(light_sensor_pin);
    delay(5);
  }
  float avgAnalogValue = sum / 50.0;

  // 2. Mapeamento linear para escala percentual (%)
  // Como o ADC tem 12 bits, o valor máximo aferível é 4095
  float lightPercentage = (avgAnalogValue / 4095.0) * 100.0;

  // Trava de segurança para limites visuais no ThingsBoard
  if (lightPercentage > 100.0) lightPercentage = 100.0;
  if (lightPercentage < 0.0) lightPercentage = 0.0;

  return lightPercentage;
}
#endif