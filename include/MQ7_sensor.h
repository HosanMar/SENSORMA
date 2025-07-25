
#ifndef MQ7_SENSOR_H
#define MQ7_SENSOR_H

#define MQ7_PIN 32
#define RL 10000.0        // Resistência de carga (ohms)
#define VC 3.3            // Tensão de alimentação (V)
#define R0 620.0         // R0 calibrado em ar limpo (ajuste conforme necessário)

void initMQ7() {
  pinMode(MQ7_PIN, INPUT);
}

float readMQ7() {


  int analogValue = analogRead(MQ7_PIN);
  float Vout = analogValue * (VC / 4095.0); // tensão de saída

  if (Vout == 0) return 0.0; // evita divisão por zero

  float Rs = RL * ((VC - Vout) / Vout);
  float ratio = Rs / R0;

  // Equação aproximada do datasheet MQ-7 (curva log-log)
  float ppm = pow(10, (-1.497 * log10(ratio) + 0.518));

  // Conversão ppm → mg/m³ para o monóxido de carbono (CO)
  float mgm3 = ppm * 1.146;
  
  Serial.print("Rs: ");
  Serial.print(Rs);
  Serial.print(" | Ratio: ");
  Serial.print(ratio);
  Serial.print(" | ppm: ");
  Serial.println(ppm);

  return mgm3;
}

#endif
