#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
<<<<<<< HEAD

#include "dht11_sensor.h"
#include "ky037_sensor.h"
#include "mq135_sensor.h"
#include "mq7_sensor.h"
#include "gp2y10_sensor.h"

// WiFi e MQTT
const char* ssid = "---";
const char* password = "---";
=======
#include <HTTPClient.h>

#include "dht11_sensor.h"
#include "KY037_sensor.h"
#include "MQ135_sensor.h"
#include "MQ7_sensor.h"
#include "GP2Y10_sensor.h"
#include "ldr_sensor.h"

const char* url_api = "http://192.168.0.11:5000/salvar"; 
unsigned long ultimaLeituraBanco = 0;
const unsigned long intervaloBanco = 3600000; 

// WiFi e MQTT
const char* ssid = "brisa-3992484";
const char* password = "zcwbwerm";
>>>>>>> 4b5cabf (Database implementation using Flask and SQLAlchemy for handling sensor data from ESP32.)
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
<<<<<<< HEAD
=======
  initLDR();
}

void enviarParaBanco(String payload) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(url_api);
    http.addHeader("Content-Type", "application/json");
    
    int httpResponseCode = http.POST(payload);
    
    if (httpResponseCode > 0) {
      Serial.printf("Dados salvos no Postgres. HTTP Code: %d\n", httpResponseCode);
    } else {
      Serial.printf("Erro ao salvar no banco: %s\n", http.errorToString(httpResponseCode).c_str());
    }
    http.end();
  }
>>>>>>> 4b5cabf (Database implementation using Flask and SQLAlchemy for handling sensor data from ESP32.)
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
<<<<<<< HEAD
=======
  int light = readLDR();
>>>>>>> 4b5cabf (Database implementation using Flask and SQLAlchemy for handling sensor data from ESP32.)

  // JSON payload
  String payload = "{";
  payload += "\"temperatura\":" + String(temp) + ",";
  payload += "\"umidade\":" + String(hum) + ",";
  payload += "\"Ruído\":" + String(sound) + ",";
  payload += "\"VOC\":" + String(air_quality) + ",";
  payload += "\"CO\":" + String(co) + ",";
<<<<<<< HEAD
  payload += "\"dust\":" + String(dust);
=======
  payload += "\"dust\":" + String(dust) + ",";
  payload += "\"light\":" + String(light);
>>>>>>> 4b5cabf (Database implementation using Flask and SQLAlchemy for handling sensor data from ESP32.)
  payload += "}";

  Serial.println(payload);
  client.publish("v1/devices/me/telemetry", payload.c_str());

<<<<<<< HEAD
=======
  unsigned long tempoAtual = millis();
  if (tempoAtual - ultimaLeituraBanco >= intervaloBanco || ultimaLeituraBanco == 0) {
    ultimaLeituraBanco = tempoAtual;
    Serial.println("Enviando leitura horária para o PostgreSQL...");
    enviarParaBanco(payload);
  }

>>>>>>> 4b5cabf (Database implementation using Flask and SQLAlchemy for handling sensor data from ESP32.)
  delay(10000);
}

