#include <Wire.h>
#include <RTClib.h>
#include <Arduino.h>
#include <SPI.h>

RTC_DS3231 rtc;  // Cria uma instância do DS3231

void setup() {
  Serial.begin(115200);      // Inicializa o Monitor Serial

  if (!rtc.begin()) {
    Serial.println("Erro: DS3231 não encontrado!");
    while (1);  // Trava o programa se o RTC não for encontrado
  }

  if (rtc.lostPower()) {
    Serial.println("DS3231 perdeu energia, ajustando para a data/hora padrão.");
    // Define uma data/hora padrão (exemplo: 12 de outubro de 2024, 12:00:00)
    rtc.adjust(DateTime(2024, 10, 12, 12, 0, 0));
  }
}

void loop() {
  DateTime agora = rtc.now();  // Obtém a data e hora atuais
  float tempInterna = rtc.getTemperature();  // Obtém a temperatura do módulo

  // Imprime data e hora no formato completo
  Serial.print("Data: ");
  Serial.print(agora.day());
  Serial.print("/");
  Serial.print(agora.month());
  Serial.print("/");
  Serial.print(agora.year());
  Serial.print(" - Hora: ");
  Serial.print(agora.hour());
  Serial.print(":");
  Serial.print(agora.minute());
  Serial.print(":");
  Serial.println(agora.second());

  // Imprime temperatura interna do módulo
  Serial.print("Temperatura interna: ");
  Serial.print(tempInterna);
  Serial.println(" °C");

  // Verifica se é AM ou PM
  if (agora.hour() < 12) {
    Serial.println("Período: AM");
  } else {
    Serial.println("Período: PM");
  }

  // Exibe o número de segundos desde a meia-noite (do dia atual)
  Serial.print("Segundos desde a meia-noite: ");
  Serial.println(agora.unixtime() % 86400);

  Serial.println("---------------------------");

  delay(1000);  // Aguarda 1 segundo para a próxima leitura
}
