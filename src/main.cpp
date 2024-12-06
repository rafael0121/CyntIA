#include <WiFi.h>
#include <HTTPClient.h>
#include <FS.h>
#include <Arduino.h>
#include <Audio.h>
#include <SD.h>
#include <mic.h>
#include <sendfile.h>
#include <config.h>

 // Create Audio object
Audio audio;

bool test = true;

void setup() {
  Serial.begin(115200);
  
  // Set microSD Card CS as OUTPUT and set HIGH
  pinMode(SD_CS, OUTPUT);      
  digitalWrite(SD_CS, HIGH); 
    
  // Initialize SPI bus for microSD Card
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
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

  mic_configure();

  // Setup I2S 
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    
  // Set Volume
  audio.setVolume(100);

}

void loop() {
  Serial.println("Press any key to start record!");
  while(Serial.available() == 0) {
    if(Serial.readString() == "aa"){
      break;
    }
  }
  char *path;
  Serial.println("log: Record");
  mic_get_voice_record();
  Serial.println("log: sendFile");
  sendFile();
  /////////
  Serial.println("log: getFile");
  getfile();
  if(test) {
    delay(50);
    audio.connecttoFS(SD,"/audio.mp3");
    delay(50);
    test = false;
    Serial.println("entrei");
  }
  audio.loop();    
}
