#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

extern const char* ssid;
extern const char* password;
extern const char* mqtt_server;
extern const char* token;
extern const char* url_api;
extern const unsigned long intervaloBanco;
extern unsigned long ultimaLeituraBanco;

#endif