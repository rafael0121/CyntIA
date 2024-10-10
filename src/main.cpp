#include "FS.h"
#include "SPIFFS.h"

// Defina o pino do DAC (GPIO25)
#define DAC_PIN 25

// Função para reproduzir o áudio WAV
void playWAV(fs::FS &fs, const char *path) {
    File file = fs.open(path);
    
    if (!file) {
        Serial.println("Erro ao abrir o arquivo!");
        return;
    } else {
        Serial.println("Abri");
    }

    // Lê e descarta o cabeçalho WAV (44 bytes)
    for (int i = 0; i < 44; i++) {
        file.read();
    }

    Serial.println("Tocar");
    // Reproduz os dados de áudio
    while (file.available()) {
        uint8_t sample = file.read();  // Lê um byte do arquivo

        // Escreve o valor no DAC (valores de 0 a 255)
        dacWrite(DAC_PIN, sample);
        Serial.println("Tocando");
        // Adiciona um pequeno delay para controlar a taxa de amostragem
        delayMicroseconds(62);  // Aproximadamente 8kHz
    }

    file.close();
    Serial.println("Reprodução de áudio finalizada!");
}

void setup() {
    Serial.begin(115200);
    Serial.println("teste");
    // Inicializa o SPIFFS
    if (!SPIFFS.begin(true)) {
        Serial.println("Erro ao montar o sistema de arquivos SPIFFS!");
        return;
    }

    // Inicia a reprodução do arquivo WAV
    playWAV(SPIFFS, "/teste.wav");
}

void loop() {
    // Nada aqui, a reprodução é feita no setup.
}
