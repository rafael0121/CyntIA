#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <string.h>
#include <locale.h>

const String ssid = "";
const String password = "";
const String Gemini_Token = "";
const String Gemini_Max_Tokens = "200";
String question = "";

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  while (!Serial);

  setlocale(LC_ALL, "Portuguese");

  // wait for WiFi connection
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() 
{
  setlocale(LC_ALL, "Portuguese");
  Serial.println("");
  Serial.println("Ask your Question: ");

  while (!Serial.available());
  while (Serial.available()) {
    question = Serial.readString();
    delay(1);
  }

  question = "\"" + question + "\"";
  Serial.println("");

  Serial.print("Asking Your Question: ");
  Serial.println(question);

  HTTPClient https;

  //Serial.print("[HTTPS] begin...\n");
  if (https.begin("https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash:generateContent?key=" + (String)Gemini_Token)) {  // HTTPS

    https.addHeader("Content-Type", "application/json");
    String payload = String("{\"contents\": [{\"parts\":[{\"text\":" + question + "}]}],\"generationConfig\": {\"maxOutputTokens\": " + (String)Gemini_Max_Tokens + "}}");

    //Serial.print("[HTTPS] GET...\n");

    // start connection and send HTTP header
    int httpCode = https.POST(payload);

    // httpCode will be negative on error
    // file found at server

    if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
      String payload = https.getString();
      //Serial.println(payload);

      DynamicJsonDocument doc(1024);

      deserializeJson(doc, payload);
      String answer = doc["candidates"][0]["content"]["parts"][0]["text"];

      // For Filtering our Special Characters, WhiteSpaces and NewLine Characters
      answer.trim();
      String filteredAnswer = "";
      for (size_t i = 0; i < answer.length(); i++) {
        char c = answer[i];
        if (isalnum(c) || isspace(c)) {
          filteredAnswer += c;
        } else {
          filteredAnswer += ' ';
        }
      }
      answer = filteredAnswer;

      Serial.println("");
      Serial.println("Here is your Answer: ");
      Serial.println("");
      Serial.println(answer);
    } else {
      Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
    }
    https.end();
  } else {
    Serial.printf("[HTTPS] Unable to connect\n");
  }
  question = "";
}