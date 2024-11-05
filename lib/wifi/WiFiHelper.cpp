#include "WiFiHelper.h"
#include <Arduino.h>

void WiFiHelper::connect(const String& ssid, const String& password) {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    WiFi.begin(ssid.c_str(), password.c_str()); // Converte String para const char* para a função WiFi.begin()
    Serial.print("Connecting to ");
    Serial.println(ssid);
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    
    Serial.println("\nConnected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}
