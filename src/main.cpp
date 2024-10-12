#include "DHT.h"

#define DHT_PIN 25      // Pino conectado ao sinal do DHT11
#define DHT_TYPE DHT11  // Tipo do sensor (DHT11)

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(115200);  // Inicializa o Monitor Serial
  dht.begin();           // Inicializa o sensor DHT11
}

void loop() {
  float temperatura = dht.readTemperature();  // Lê a temperatura
  float umidade = dht.readHumidity();         // Lê a umidade

  // Verifica se a leitura foi bem-sucedida
  if (isnan(temperatura) || isnan(umidade)) {
    Serial.println("Falha ao ler do sensor DHT11!");
  } else {
    Serial.print("Temperatura: ");
    Serial.print(temperatura);
    Serial.println(" °C");

    Serial.print("Umidade: ");
    Serial.print(umidade);
    Serial.println(" %");
  }

  delay(2000);  // Aguarda 2 segundos para próxima leitura
}
