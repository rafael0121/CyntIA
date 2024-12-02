#include <driver/i2s.h>
#include <Arduino.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ESP32Ping.h>
#include <FS.h>
#include <SD.h>
#include <Audio.h>

// Local library
#include "config.h"
#include "mic.h"
#include "sendfile.h"

 // Create Audio object
Audio audio;

void connectWiFi() {
  Serial.print("Conectando ao Wi-Fi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi conectado.");
}

void setup() {
  // Set up Serial Monitor
  Serial.begin(9600);
  Serial.println(" ");

  // Set up I2S microphone
  mic_configure();
  connectWiFi();


  // Start microSD Card
  if(!SD.begin(SD_CS))
  {
    Serial.println("Error accessing microSD card!");
    while(true); 
  }

  // Setup I2S 
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    
  // Set Volume
  audio.setVolume(100);
 
  // Open music file
  audio.connecttoFS(SD,"/audio.mp3");

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

  bool finish = false;

  while(!finish) {
    finish = getfile();
  }

  audio.loop();    
}