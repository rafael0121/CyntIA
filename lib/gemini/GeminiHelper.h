#ifndef GEMINI_HELPER_H
#define GEMINI_HELPER_H

#include <Arduino.h>

class GeminiHelper {
public:
    GeminiHelper(const String& token, const String& maxTokens);
    String askQuestion(const String& question);

private:
    String token;
    String maxTokens;
};

#endif