#include <WiFi.h>
#include <HTTPClient.h>
#include <FS.h>
#include <api.h>
#include <Arduino.h>
#include <Audio.h>
#include <SD.h>

// Configurações Wi-Fi
const char* ssid = "Carcara";
const char* password = "67506750";

// URLs
String apiElevenLabs = "https://api.elevenlabs.io/v1/text-to-speech/33B4UnXyTNbgLmdEDh5P";

const char* mp3_file_path = "/audio1.mp3";

// microSD Card Reader connections
#define SD_CS          5
#define SPI_MOSI      23 
#define SPI_MISO      19
#define SPI_SCK       18
 
// I2S Connections
#define I2S_DOUT      25
#define I2S_BCLK      27
#define I2S_LRC       26

 // Create Audio object
Audio audio;

// Função para obter o MP3 da API e salvar na SPIFFS
bool getMP3FromElevenLabs(String text) {
  WiFiClientSecure client;
  HTTPClient http;

  client.setInsecure();  // Ignorar SSL (para testes)

  http.begin(apiElevenLabs);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("xi-api-key", ELEVEN_LABS_API_KEY);

  // Corpo da requisição JSON com o texto
  String payload = "{\"text\":\"" + text + "\", \"model_id\":\"eleven_turbo_v2_5\", \"language_code\":\"pt\"}";

  http.setTimeout(5000);

  int httpResponseCode = http.POST(payload);

  if (httpResponseCode == 200) {
    // Abrir arquivo na SPIFFS para gravação
    File mp3File = SD.open(mp3_file_path, FILE_WRITE);
    if (!mp3File) {
      Serial.println("Erro ao abrir arquivo para gravação.");
      http.end();
      return false;
    }

    // Ler o stream da resposta e gravar no arquivo
    WiFiClient* stream = http.getStreamPtr();
    size_t written = 0;
    uint8_t buffer[128];  // Buffer de 128 bytes

    while (stream->available()) {
      int bytesRead = stream->readBytes(buffer, sizeof(buffer));
      written += mp3File.write(buffer, bytesRead);
      delay(50);
    }

    mp3File.close();
    Serial.printf("MP3 salvo com sucesso (%d bytes).\n", written);
    http.end();
    return true;
  } else {
    Serial.printf("Erro na requisição GET: %d\n", httpResponseCode);
    http.end();
    return false;
  }
}

void setup() {
  Serial.begin(115200);

  // Set microSD Card CS as OUTPUT and set HIGH
  pinMode(SD_CS, OUTPUT);      
  digitalWrite(SD_CS, HIGH); 
    
  // Initialize SPI bus for microSD Card
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }
  Serial.println("WiFi conectado!");

  // Start microSD Card
  if(!SD.begin(SD_CS))
  {
    Serial.println("Error accessing microSD card!");
    while(true); 
  }

  // Texto a ser convertido em áudio
  String textoParaConverter = "A baleia-azul é o maior animal do mundo, podendo atingir mais de 30 metros de comprimento.";

  while(!getMP3FromElevenLabs(textoParaConverter)) {
    break;
  }

  // Setup I2S 
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    
  // Set Volume
  audio.setVolume(100);
 
  // Open music file
  audio.connecttoFS(SD,"/audio1.mp3");
  
}

void loop() {
  audio.loop();    
}
