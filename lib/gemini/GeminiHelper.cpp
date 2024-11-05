#include "GeminiHelper.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>

GeminiHelper::GeminiHelper(const String& token, const String& maxTokens) 
    : token(token), maxTokens(maxTokens) {}

String GeminiHelper::askQuestion(const String& question) {
    HTTPClient https;
    String apiUrl = "https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash:generateContent?key=" + token;

    if (https.begin(apiUrl)) {
        https.addHeader("Content-Type", "application/json");
        String payload = "{\"contents\": [{\"parts\":[{\"text\":\"" + question + "\"}]}],\"generationConfig\": {\"maxOutputTokens\": " + maxTokens + "}}";

        int httpCode = https.POST(payload);

        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            String response = https.getString();
            https.end();

            DynamicJsonDocument doc(1024);
            deserializeJson(doc, response);
            String answer = doc["candidates"][0]["content"]["parts"][0]["text"];
            answer.trim();

            return answer;
        } else {
            https.end();
            return "[HTTPS] GET failed, error: " + https.errorToString(httpCode);
        }
    } else {
        return "[HTTPS] Unable to connect";
    }
}