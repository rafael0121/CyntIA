#include <Arduino.h>
#define SENSOR_PIN 25  // Pino onde o SW-420 está conectado

void setup() {
  Serial.begin(115200);         // Inicializa o Monitor Serial
  pinMode(SENSOR_PIN, INPUT);    // Define o pino como entrada
}

void loop() {
  int estado = digitalRead(SENSOR_PIN);  // Lê o estado do sensor

  if (estado == LOW) {
    Serial.println("Nenhuma vibração.");  // Sinal ativo em LOW
  } else {
    Serial.println("Vibração detectada!");  // Sem vibração
  }

  delay(500);  // Aguarda 500ms antes da próxima leitura
}
