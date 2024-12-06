#include <HTTPClient.h>
#include "esp_http_client.h"
#include <Arduino.h>
#include <SPIFFS.h>
#include <SD.h>

#include "config.h"
#include "sendfile.h"

bool sendFile(){
  WiFiClient client;
  HTTPClient http;

  // Iniciar a requisição HTTP para o servidor de upload
  http.begin(SERVER_URL_UPLOAD); // Endpoint PUT
  http.addHeader("Content-Type", "application/octet-stream");

  // Abrir o arquivo de áudio para leitura
  File audioFile = SPIFFS.open("/audio.wav", "r");
  if (!audioFile) {
    Serial.println("Erro ao abrir o arquivo.");
    return false;
  }

  http.setTimeout(20000);
  client.setTimeout(50000);

  // Enviar o arquivo usando o método PUT
  int httpResponseCode = http.sendRequest("PUT", &audioFile, audioFile.size());

  // Verificar o código de resposta da requisição
  if (httpResponseCode > 0) {
    Serial.printf("Código de resposta HTTP: %d\n", httpResponseCode);

    File mp3file = SD.open("/audio.mp3", FILE_WRITE);
    if (!mp3file) {
      Serial.println("Falha ao abrir o arquivo para escrita.");
    } else {
      WiFiClient * stream = http.getStreamPtr();
      while (stream->available()) {
        mp3file.write(stream->read());
        delay(10);
      }
      mp3file.close(); // Fecha o arquivo no SD
      Serial.println("Arquivo MP3 salvo no cartão SD!");
    }

    // Finalizar a requisição
    http.end();
    audioFile.close();

    return true;
  } else {
    Serial.printf("Código de resposta HTTP: %d\n", httpResponseCode);
    return false;
  }

  return false;
}