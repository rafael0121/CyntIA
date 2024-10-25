#include <WiFi.h>
#include <HTTPClient.h>
#include <FS.h>
#include <SPIFFS.h>
#include <api.h>

// Configurações Wi-Fi
const char* ssid = "WT-MIGUEL";
const char* password = "GomesCampos9862709402";

// URLs
String apiElevenLabs = "https://api.elevenlabs.io/v1/text-to-speech/33B4UnXyTNbgLmdEDh5P";

// Caminho do arquivo MP3 na SPIFFS
const char* mp3_file_path = "/audio.mp3";

// Inicializa a SPIFFS
void initSPIFFS() {
  if (!SPIFFS.begin(true)) {
    Serial.println("Erro ao inicializar SPIFFS");
    while (true);
  }
  Serial.println("SPIFFS inicializado com sucesso.");
}

// Função para obter o MP3 da API e salvar na SPIFFS
bool getMP3FromElevenLabs(String text) {
  WiFiClientSecure client;
  HTTPClient http;

  client.setInsecure();  // Ignorar SSL (para testes)

  http.begin(apiElevenLabs);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("xi-api-key", ELEVEN_LABS_API_KEY);

  // Corpo da requisição JSON com o texto
  String payload = "{\"text\":\"" + text + "\"}";

  int httpResponseCode = http.POST(payload);

  if (httpResponseCode == 200) {
    // Abrir arquivo na SPIFFS para gravação
    File mp3File = SPIFFS.open(mp3_file_path, FILE_WRITE);
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
  initSPIFFS();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }
  Serial.println("WiFi conectado!");


  // Texto a ser convertido em áudio
  String textoParaConverter = "Oi. Meu nome é CyntIA. É muito legal te conhecer! Vamo ser grandes amigos emoji sorrindo";

  //getMP3FromElevenLabs(textoParaConverter)

  File root = SPIFFS.open("/");
 
  File file = root.openNextFile();
 
  while(file){
 
      Serial.print("FILE: ");
      Serial.println(file.name());
 
      file = root.openNextFile();
  }
  
}

void loop() {
  
}
