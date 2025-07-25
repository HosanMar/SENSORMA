#ifndef MQ135_SENSOR_H
#define MQ135_SENSOR_H

#define MQ135_PIN 35
#define RL_MQ135 10000.0 // Resistência de carga RL (ohms)
#define VC_MQ135 3.3     // Tensão de alimentação
#define R0_MQ135 20000.0 // R0 calibrado em ar limpo (ajuste após calibração)

void initMQ135() {
  pinMode(MQ135_PIN, INPUT);
}

float readMQ135() {
  int analogValue = analogRead(MQ135_PIN);
  float Vout = analogValue * (VC_MQ135 / 4095.0);

  if (Vout <= 0.0) return 0.0; // Evita divisão por zero

  float Rs = RL_MQ135 * ((VC_MQ135 - Vout) / Vout);
  float ratio = Rs / R0_MQ135;

  // Curva aproximada do datasheet para amônia, benzeno, CO2, etc.
  // ppm = 10 ^ (a * log10(ratio) + b)
  float a = -0.42;  // Inclinação típica para CO2 no gráfico Rs/R0 vs ppm
  float b = 0.36;   // Intercepto

  float ppm = pow(10, a * log10(ratio) + b); // ppm estimado

  return ppm;
}

#endif
