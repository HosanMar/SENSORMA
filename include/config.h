#ifndef CONFIG_H
#define CONFIG_H

#define V_ADC 3.3         // Tensão máxima de leitura do ADC da ESP32
#define V_SENSOR 5.0      // Tensão de alimentação real do sensor (VCC)

extern const char* ssid;
extern const char* password;
extern const char* mqtt_server;
extern const char* token;
extern const char* url_api;
extern const unsigned long intervaloBanco;
extern unsigned long ultimaLeituraBanco;

#endif