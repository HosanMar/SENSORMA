#ifndef KY037_SENSOR_H
#define KY037_SENSOR_H

#include <Arduino.h>

#define KY037_PIN 34  // Pino analógico

void initKY037() {
  pinMode(KY037_PIN, INPUT);
}

int readKY037() {
  return analogRead(KY037_PIN);
}

#endif
