// Credenciais do Wi-Fi
#define WIFI_SSID "rafael"
#define WIFI_PASSWORD "rafaelrx"

// microSD Card Reader connections
#define SD_CS          5
#define SPI_MOSI      23 
#define SPI_MISO      19
#define SPI_SCK       18
 
// speaker Connections
#define I2S_DOUT      22
#define I2S_BCLK      26
#define I2S_LRC       25

#define I2S_WS 27
#define I2S_SD 32
#define I2S_SCK 33
// Use I2S Processor 0 - <#
#define I2S_PORT I2S_NUM_1

// Endereço do servidor local
#define SERVER_URL_UPLOAD "http:// 192.168.146.103:3000/upload" // Substitua pelo IP do servidor