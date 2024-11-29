#include <HTTPClient.h>
#include <Arduino.h>
#include <SPIFFS.h>

#include "config.h"
#include "sendfile.h"


void sendFile() {
  WiFiClient client;
  HTTPClient http;

  http.begin(SERVER_URL); // Endpoint POST
  http.addHeader("Content-Type", "application/octet-stream");

  // Abre o arquivo para leitura
  File audioFile = SPIFFS.open("/audio.wav", "r");
  if (!audioFile) {
    Serial.println("Erro ao abrir o arquivo.");
    return;
  }
  
  int httpResponseCode = http.sendRequest("PUT", &audioFile, audioFile.size());
  
// Finalizar a requisição
  if (httpResponseCode > 0) {
    Serial.printf("HTTP Response code: %d\n", httpResponseCode);
  } else {
    Serial.printf("Erro ao enviar o arquivo: %s\n", http.errorToString(httpResponseCode).c_str());
  }
  
  http.end();
  audioFile.close();
}

