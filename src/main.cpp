#include <Arduino.h>
#include "AudioFileSourceSPIFFS.h"
#include "AudioGeneratorWAV.h"
#include "AudioOutputI2S.h"
#include <SPIFFS.h>

AudioGeneratorWAV *wav;
AudioFileSourceSPIFFS *file;
AudioOutputI2S *out;
void setup()
{
  Serial.begin(115200);
  delay(1000);
  SPIFFS.begin();
  file = new AudioFileSourceSPIFFS("/teste.wav");
  out = new AudioOutputI2S();
  wav = new AudioGeneratorWAV();
  wav->begin(file, out);
}

void loop()
{
  if (wav->isRunning()) {
    if (!wav->loop()) wav->stop(); 
  } else {
    Serial.printf("MP3 done\n");
    delay(1000);
  }
}