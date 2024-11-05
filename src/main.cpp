#include <Arduino.h>
#include "WiFiHelper.h"
#include "GeminiHelper.h"
#include <locale.h>

const String ssid = "";
const String password = "";
const String Gemini_Token = "";
const String Gemini_Max_Tokens = "200";
String question = "";

WiFiHelper wifiHelper;
GeminiHelper geminiHelper(Gemini_Token, Gemini_Max_Tokens);

void setup() {
    Serial.begin(115200);
    while (!Serial);

    setlocale(LC_ALL, "Portuguese");
    wifiHelper.connect(ssid, password);
}

void loop() {
    setlocale(LC_ALL, "Portuguese");
    Serial.println("");
    Serial.println("Ask your Question: ");

    while (!Serial.available());
    while (Serial.available()) {
        question = Serial.readString();
        delay(1);
    }

    Serial.println("Asking Your Question...");
    String answer = geminiHelper.askQuestion(question);

    Serial.println("Answer: ");
    Serial.println(answer);

    question = "";
}