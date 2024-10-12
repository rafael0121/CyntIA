#include <Arduino.h>
#define SENSOR_PIN 18  // Pin onde o KY-031 está conectado

void setup() {
  Serial.begin(115200);  // Inicializa o Monitor Serial
  pinMode(SENSOR_PIN, INPUT);  // Define o pino como entrada
}

void loop() {
  int estado = digitalRead(SENSOR_PIN);  // Lê o estado do sensor

  if (estado == HIGH) {
    Serial.println("Batida detectada!");  // Imprime se houve batida
  } else {
    Serial.println("Nenhuma batida.");  // Imprime se não houve batida
  }

  delay(500);  // Pequeno atraso para evitar spam no Monitor Serial
}
