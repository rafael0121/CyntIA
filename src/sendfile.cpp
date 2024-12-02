#include <HTTPClient.h>
#include <Arduino.h>
#include <SPIFFS.h>
#include <SD.h>

#include "config.h"
#include "sendfile.h"


void sendFile() {
  WiFiClient client;
  HTTPClient http;

  http.begin(SERVER_URL_DOWNLOAD); // Endpoint POST
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


bool getfile() {

  HTTPClient http;
  http.begin(SERVER_URL_DOWNLOAD);    // Inicia a conexão HTTP
  int httpCode = http.GET(); // Envia a requisição GET

  if (httpCode == 200) {  // Se a resposta for 200 (OK)
    Serial.println("Arquivo encontrado. Iniciando download...");

    // Abre o arquivo no cartão SD para gravação
    File file = SD.open("/audio.mp3", FILE_WRITE);
    if (!file) {
      Serial.println("Falha ao abrir o arquivo para escrita.");
      return false;
    }

    // Obtém o stream de dados e escreve no cartão SD
    WiFiClient * stream = http.getStreamPtr();
    while (stream->available()) {
      file.write(stream->read());
    }

    file.close(); // Fecha o arquivo no SD
    Serial.println("Arquivo MP3 salvo no cartão SD!");

    http.end();  // Finaliza a requisição HTTP

    return true;
  } else {
    Serial.printf("Falha ao baixar o arquivo, código de erro: %d\n", httpCode);
  }

  http.end();  // Finaliza a requisição HTTP

  return false;
}