#ifndef RFIDREADER_H
#define RFIDREADER_H

#include <SPI.h>
#include <MFRC522.h>
#include "FunctionalInterrupt.h"

// Pin definitions for RFID reader
#define RST_PIN         22          
#define IRQ_PIN         21          
#define SDA_PIN         5           
#define MOSI_PIN        23          
#define MISO_PIN        19          
#define SCLK_PIN        18          

class RFIDReader {
public:
    void init();
    void readCard();
    
private:
    MFRC522 mfrc522;
    MFRC522::MIFARE_Key key;
    volatile bool bNewInt;
    byte regVal;

    void IRAM_ATTR readTag();
    void IRAM_ATTR removedTag();
    void activateRec();
    void clearInt();
};

#endif // RFIDREADER_H