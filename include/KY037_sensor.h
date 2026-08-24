#ifndef KY037_SENSOR_H
#define KY037_SENSOR_H

<<<<<<< HEAD
=======
#include <Arduino.h>

>>>>>>> 4b5cabf (Database implementation using Flask and SQLAlchemy for handling sensor data from ESP32.)
#define KY037_PIN 34  // Pino analógico

void initKY037() {
  pinMode(KY037_PIN, INPUT);
}

int readKY037() {
  return analogRead(KY037_PIN);
}

#endif
