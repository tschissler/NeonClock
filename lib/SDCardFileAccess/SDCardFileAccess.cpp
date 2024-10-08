#include "SDCardFileAccess.h"
#include <Arduino.h> // Include Arduino for Serial

#define welcomeFile "/neonclock.txt"

SDCardFileAccess::SDCardFileAccess() : sd_spi(HSPI) {}

void SDCardFileAccess::init() {
    sd_spi.begin(SD_SCLK_PIN, SD_MISO_PIN, SD_MOSI_PIN, SD_CS_PIN);
    if (!SD.begin(SD_CS_PIN, sd_spi)) {
        Serial.println("Initialization of SD-Card Reader failed!");
        return;
    }
    Serial.println("Initialization of SD-Card Reader done.");

    if (!readFile(welcomeFile)) {
        writeFile(welcomeFile, "Welcome to NeonClock!");
        if (!readFile(welcomeFile)) 
            Serial.println(F("SD card is not working! Please check if a card is inserted which is formated with FAT32."));
        else
            Serial.println(F("SD card is working properly!"));
    } else
        Serial.println(F("SD card is working properly!"));
}

bool SDCardFileAccess::readFile(const char* filename) {
    File file = SD.open(filename);
    if (!file) {
        Serial.println("Failed to open file for reading");
        return false;
    }

    while (file.available()) {
        Serial.write(file.read());
    }
    file.close();
    return true;
}

bool SDCardFileAccess::writeFile(const char* filename, const char* data) {
    File file = SD.open(filename, FILE_WRITE);
    if (!file) {
        Serial.println("Failed to open file for writing");
        return false;
    }

    file.println(data);
    file.close();
    return true;
}