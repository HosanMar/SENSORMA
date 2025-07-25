#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

#include "dht11_sensor.h"
#include "ky037_sensor.h"
#include "mq135_sensor.h"
#include "mq7_sensor.h"
#include "gp2y10_sensor.h"

// WiFi e MQTT
const char* ssid = "brisa-3992484";
const char* password = "zcwbwerm";
const char* mqtt_server = "demo.thingsboard.io";
const char* token = "M3p71rB61chScJ7nyl7g";

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado.");
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32Client", token, NULL)) {
      Serial.println("Conectado ao ThingsBoard");
    } else {
      Serial.print("Erro rc=");
      Serial.println(client.state());
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);

  initDHT();
  initKY037();
  initMQ135();
  initMQ7();
  initGP2Y10();
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  float temp = readTemperature();
  float hum = readHumidity();
  int sound = readKY037();
  float air_quality = readMQ135();
  float co = readMQ7();
  float dust = readGP2Y10();

  // JSON payload
  String payload = "{";
  payload += "\"temperatura\":" + String(temp) + ",";
  payload += "\"umidade\":" + String(hum) + ",";
  payload += "\"Ruído\":" + String(sound) + ",";
  payload += "\"VOC\":" + String(air_quality) + ",";
  payload += "\"CO\":" + String(co) + ",";
  payload += "\"dust\":" + String(dust);
  payload += "}";

  Serial.println(payload);
  client.publish("v1/devices/me/telemetry", payload.c_str());

  delay(10000);
}

