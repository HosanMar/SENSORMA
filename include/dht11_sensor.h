#ifndef DHT_SENSOR_H
#define DHT_SENSOR_H

#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void initDHT() {
  dht.begin();
}

float readTemperature() {
  return dht.readTemperature();
}

float readHumidity() {
  return dht.readHumidity();
}

#endif
