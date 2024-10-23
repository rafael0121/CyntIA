#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "WT-MIGUEL";
const char* password = "GomesCampos9862709402";
const char* APIURL = "https://api.elevenlabs.io/v1/text-to-speech/33B4UnXyTNbgLmdEDh5P";
const char* apiKey = "sk_d0ff6eca4b0dc87d502c6f6ca36a897f73f5a6df47cef108";
const char* backendURL = "http://192.168.1.3:3000/upload"; // URL do backend

String question = "";

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("Conectando ao WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConectado!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void enviarAudioParaBackend(WiFiClient* stream) {
  WiFiClient client;
  const char* host = "192.168.1.3";  // IP do servidor backend
  const int port = 3000;

  if (!client.connect(host, port)) {
    Serial.println("Falha ao conectar ao backend");
    return;
  }

  String boundary = "----ESP32Boundary";
  String bodyStart = "--" + boundary + "\r\n"
                     "Content-Disposition: form-data; name=\"file\"; filename=\"audio.mp3\"\r\n"
                     "Content-Type: audio/mpeg\r\n\r\n";
  String bodyEnd = "\r\n--" + boundary + "--\r\n";

  int contentLength = bodyStart.length() + stream->available() + bodyEnd.length();

  // Envia o cabeçalho HTTP
  client.printf(
      "POST /upload HTTP/1.1\r\n"
      "Host: %s\r\n"
      "Content-Type: multipart/form-data; boundary=%s\r\n"
      "Content-Length: %d\r\n"
      "Connection: close\r\n\r\n",
      host, boundary.c_str(), contentLength);

  client.setTimeout(20000);
  client.print(bodyStart);  // Envia o início do corpo

  // Envia o áudio em chunks
  uint8_t buffer[512];
  while (stream->available()) {
    size_t len = stream->read(buffer, sizeof(buffer));
    client.write(buffer, len);
    delay(10);  // Pausa para evitar sobrecarga
  }

  client.print(bodyEnd);  // Envia o final do corpo
  client.setTimeout(20000);
  // Lê a resposta do servidor
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") break;  // Fim do cabeçalho HTTP
  }
  client.setTimeout(20000);
  String response = client.readString();
  Serial.println("Resposta do servidor:");
  Serial.println(response);

  client.stop();  // Fecha a conexão corretamente
}





void loop() {
  Serial.println("\nDigite algo para ser escutado:");

  while (!Serial.available());
  question = Serial.readString();
  question.trim();
  Serial.print("Pergunta enviada: ");
  Serial.println(question);

  // Monta o JSON corretamente
  String payload = "{";
  payload += "\"text\":\"" + question + "\",";
  payload += "\"model_id\":\"eleven_multilingual_v2\",";
  payload += "\"voice_settings\":{\"stability\":0.5,\"similarity_boost\":0.5}";
  payload += "}";

  // Faz a requisição HTTPS para Eleven Labs
  HTTPClient https;
  if (https.begin(APIURL)) {
    https.addHeader("Accept", "audio/mpeg");
    https.addHeader("Content-Type", "application/json");
    https.addHeader("xi-api-key", apiKey);
    https.setTimeout(30000);
    int httpCode = https.POST(payload);
    if (httpCode == HTTP_CODE_OK) {
      Serial.println("Recebendo áudio...");

      // Lê o stream de áudio
      WiFiClient* stream = https.getStreamPtr();


      Serial.println("antes de passar");
      // Envia o áudio para o backend
      enviarAudioParaBackend(stream);
      Serial.println("passou");
    } else {
      Serial.printf("Erro na requisição: %s\n", https.errorToString(httpCode).c_str());
    }
    https.end();
  } else {
    Serial.println("Falha na conexão HTTPS.");
  }

  question = "";  // Reseta a pergunta
}