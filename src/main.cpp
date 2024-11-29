#include <driver/i2s.h>
#include <Arduino.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ESP32Ping.h>

// Local library
#include "config.h"
#include "mic.h"
#include "sendfile.h"

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
}