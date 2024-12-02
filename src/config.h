// Credenciais do Wi-Fi
#define WIFI_SSID "rafael"
#define WIFI_PASSWORD "rafaelrx"

// Connections to INMP441 I2S microphone
#define I2S_WS 25
#define I2S_SD 32
#define I2S_SCK 33
// Use I2S Processor 0 - <#
#define I2S_PORT I2S_NUM_0

// microSD Card Reader connections
#define SD_CS          5
#define SPI_MOSI      23 
#define SPI_MISO      19
#define SPI_SCK       18
 
// I2S Connections
#define I2S_DOUT      25
#define I2S_BCLK      27
#define I2S_LRC       26

// microSD Card Reader connections
#define SD_CS          5
#define SPI_MOSI      23 
#define SPI_MISO      19
#define SPI_SCK       18
 
// I2S Connections
#define I2S_DOUT      25
#define I2S_BCLK      27
#define I2S_LRC       26

// Endereço do servidor local
#define SERVER_URL_UPLOAD "http://192.168.1.90:3000/upload" // Substitua pelo IP do servidor
#define SERVER_URL_DOWNLOAD "http://192.168.1.90:3000/download" // Substitua pelo IP do servidor