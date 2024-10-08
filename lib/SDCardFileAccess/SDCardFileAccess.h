#ifndef SDCARDFILEACCESS_H
#define SDCARDFILEACCESS_H

#include <SD.h>
#include <SPI.h>

#define SD_CS_PIN       15
#define SD_MOSI_PIN     13
#define SD_MISO_PIN     12
#define SD_SCLK_PIN     14

class SDCardFileAccess {
public:
    SDCardFileAccess();
    void init();
    bool readFile(const char* filename);
    bool writeFile(const char* filename, const char* data);

private:
    SPIClass sd_spi;
};

#endif // SDCARDFILEACCESS_H