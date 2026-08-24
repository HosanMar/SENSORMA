#ifndef LDR_SENSOR_H
#define LDR_SENSOR_H

#include <Arduino.h>
#define light_sensor_pin 36

void initLDR() {
  pinMode(light_sensor_pin, INPUT);
  analogSetAttenuation(ADC_11db);
}

int readLDR() {
  int ldrValue = analogRead(light_sensor_pin);
  return ldrValue;
}

#endif