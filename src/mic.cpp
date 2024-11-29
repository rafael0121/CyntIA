#include <driver/i2s.h>
#include <Arduino.h>
#include <SPIFFS.h>
#include <config.h>
 
// Define input buffer length
#define bufferLen 64
#define SAMPLE_RATE 16000 // Taxa de amostragem
#define BITS_PER_SAMPLE 16
#define CHANNELS 1 // Mono
#define BUFFER_SIZE 1024


int16_t sBuffer[bufferLen];

//=======================================================================================================
// Private Functions
//=======================================================================================================
 
void setupI2S() {
  i2s_config_t i2s_config = {
    .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = i2s_bits_per_sample_t(BITS_PER_SAMPLE),
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S),
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = BUFFER_SIZE
  };

  i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_SD
  };

  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config);
}

void createWavHeader(File &file, int dataSize) {
  // Escrevendo o cabeçalho WAV em formato binário
  file.write((const uint8_t *)"RIFF", 4); // Chunk ID
  uint32_t chunkSize = dataSize + 36;    // Tamanho total do arquivo menos 8 bytes
  file.write((const uint8_t *)&chunkSize, 4);
  file.write((const uint8_t *)"WAVE", 4); // Formato

  // Subchunk 1 "fmt "
  file.write((const uint8_t *)"fmt ", 4); 
  uint32_t subchunk1Size = 16; // Tamanho do subchunk 1
  file.write((const uint8_t *)&subchunk1Size, 4);
  uint16_t audioFormat = 1; // PCM
  file.write((const uint8_t *)&audioFormat, 2);
  uint16_t numChannels = 1; // Mono
  file.write((const uint8_t *)&numChannels, 2);
  uint32_t sampleRate = 16000; // Taxa de amostragem
  file.write((const uint8_t *)&sampleRate, 4);
  uint32_t byteRate = sampleRate * numChannels * 16 / 8; // Byte rate
  file.write((const uint8_t *)&byteRate, 4);
  uint16_t blockAlign = numChannels * 16 / 8; // Alinhamento dos blocos
  file.write((const uint8_t *)&blockAlign, 2);
  uint16_t bitsPerSample = 16; // 16 bits por amostra
  file.write((const uint8_t *)&bitsPerSample, 2);

  // Subchunk 2 "data"
  file.write((const uint8_t *)"data", 4);
  file.write((const uint8_t *)&dataSize, 4); // Tamanho do subchunk 2 (áudio)
}

void recordAudio() {
  if (!SPIFFS.begin(true)) {
    Serial.println("log: SPIFFS Mount Failed");
    return;
  }

  File audioFile = SPIFFS.open("/audio.wav", FILE_WRITE);

  if (!audioFile) {
    Serial.println("log: Falha ao criar arquivo WAV");
    return;
  }

  createWavHeader(audioFile, 0); // Cabeçalho inicial vazio

  size_t bytesRead;
  uint8_t i2sData[BUFFER_SIZE];
  int totalBytes = 0;

  Serial.println("log: Iniciando gravação por 5 segundos...");
  unsigned long start = millis();
  while (millis() - start < 10000) {
    i2s_read(I2S_NUM_0, i2sData, BUFFER_SIZE, &bytesRead, portMAX_DELAY);
    audioFile.write(i2sData, bytesRead);
    totalBytes += bytesRead;
  }

  audioFile.seek(4);
  audioFile.write((uint8_t *)&totalBytes, 4); // Atualiza tamanho no cabeçalho
  audioFile.seek(40);
  audioFile.write((uint8_t *)&totalBytes, 4);

  audioFile.close();
  Serial.println("log: Gravação concluída!");
}

//========================================================================================================
// Public Functions
//========================================================================================================

void mic_configure() {
  setupI2S();
}

char * mic_get_voice_record() {
  recordAudio();
  return "/audio.wav";
}