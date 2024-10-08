/**
 * ----------------------------------------------------------------------------
 * This is a MFRC522 library example; see https://github.com/miguelbalboa/rfid
 * for further details and other examples.
 * 
 * NOTE: The library file MFRC522.h has a lot of useful info. Please read it.
 * 
 * Released into the public domain.
 * ----------------------------------------------------------------------------
 * Minimal example how to use the interrupts to read the UID of a MIFARE Classic PICC
 * (= card/tag).
 * 
 * 
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        3                10
 * IRQ         ?            ?             ?         ?          2                10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 *
 * More pin layouts for other boards can be found here: https://github.com/miguelbalboa/rfid#pin-layout
 * 
 */

#include <SPI.h>
#include <MFRC522.h>
#include <FastLED.h>
#include "RTClib.h"
#include <SD.h>

#define NUM_LEDS 180
#define DATA_PIN 16

// Define the array of leds
CRGB leds[NUM_LEDS];

#define RST_PIN         22          // Configurable, see typical pin layout above
#define IRQ_PIN         21          // Configurable, depends on hardware
#define SDA_PIN         5           // Configurable, see typical pin layout above
#define MOSI_PIN        23          // Configurable, see typical pin layout above
#define MISO_PIN        19          // Configurable, see typical pin layout above
#define SCLK_PIN        18         

// SD Card
#define SD_CS_PIN       15
#define SD_MOSI_PIN     13
#define SD_MISO_PIN     12
#define SD_SCLK_PIN     14

SPIClass sd_spi(HSPI);  // Use HSPI for Device 1
SPIClass rfid_spi(VSPI);  // Use VSPI for Device 2

MFRC522 mfrc522(SDA_PIN, RST_PIN);   // Create MFRC522 instance.

MFRC522::MIFARE_Key key;

volatile bool bNewInt = false;
byte regVal = 0x7F;
void activateRec(MFRC522 mfrc522);
void clearInt(MFRC522 mfrc522);

RTC_DS1307 rtc;

File myFile;

/**
 * MFRC522 interrupt serving routine
 */
void readCard() {
  bNewInt = true;
  Serial.println(F("Interrupt routine"));
}

void removedCard() {
  bNewInt = true;
  Serial.println(F("Card removed"));
}

// void setup() {
//   Serial.begin(115200); 

//   while (!SD.begin(SD_CS_PIN)) {
//     Serial.println(F("SD CARD FAILED, OR NOT PRESENT!"));
//     delay(1000);
//   }

//   Serial.println(F("SD CARD INITIALIZED."));

//   if (!SD.exists("/esp32.txt")) {
//     Serial.println(F("esp32.txt doesn't exist. Creating esp32.txt file..."));
//     // create a new file by opening a new file and immediately close it
//     myFile = SD.open("/esp32.txt", FILE_WRITE);
//     myFile.close();
//   }

//   // recheck if file is created or not
//   if (SD.exists("/esp32.txt"))
//     Serial.println(F("esp32.txt exists on SD Card."));
//   else
//     Serial.println(F("esp32.txt doesn't exist on SD Card."));
// }
/**
 * Initialize.
 */
void setup() {
  Serial.begin(115200); // Initialize serial communications with the PC
  while (!Serial);   
  
  Serial.println(F("Start setup"));

  SPI.begin(SCLK_PIN, MISO_PIN, MOSI_PIN, SDA_PIN);          // Init SPI bus
  sd_spi.begin(SD_SCLK_PIN, SD_MISO_PIN, SD_MOSI_PIN, SD_CS_PIN); // Init SPI bus

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
  }
  Serial.println("RTC module found");
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(2024, 10, 8, 9, 12, 0));
  }
  Serial.print("RTC is running - Current time: ");
  Serial.println(rtc.now().timestamp(DateTime::TIMESTAMP_FULL));

  if (!SD.begin(SD_CS_PIN, sd_spi)) {
    Serial.println(F("SD CARD FAILED, OR NOT PRESENT!"));
  }
  else {
    Serial.println(F("SD CARD INITIALIZED."));
  }

  if (!SD.exists("/esp32.txt")) {
    Serial.println(F("esp32.txt doesn't exist. Creating esp32.txt file..."));
    // create a new file by opening a new file and immediately close it
    myFile = SD.open("/esp32.txt", FILE_WRITE);
    myFile.close();
  }

  // recheck if file is created or not
  if (SD.exists("/esp32.txt"))
    Serial.println(F("esp32.txt exists on SD Card."));
  else
    Serial.println(F("esp32.txt doesn't exist on SD Card."));

  mfrc522.PCD_Init(); // Init MFRC522 card

  Serial.println(F("*****************************"));
  Serial.println(F("MFRC522 Digital self test"));
  Serial.println(F("*****************************"));
  mfrc522.PCD_DumpVersionToSerial();  // Show version of PCD - MFRC522 Card Reader
  Serial.println(F("-----------------------------"));
  Serial.println(F("Only known versions supported"));
  Serial.println(F("-----------------------------"));
  Serial.println(F("Performing test..."));
  bool result = mfrc522.PCD_PerformSelfTest(); // perform the test
  Serial.println(F("-----------------------------"));
  Serial.print(F("Result: "));
  if (result)
    Serial.println(F("OK"));
  else
    Serial.println(F("DEFECT or UNKNOWN"));
  Serial.println();

  /* read and printout the MFRC522 version (valid values 0x91 & 0x92)*/
  Serial.print(F("Ver: 0x"));
  byte readReg = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);
  Serial.println(readReg, HEX);

  /* setup the IRQ pin*/
  pinMode(IRQ_PIN, INPUT_PULLUP);

  /*
   * Allow the ... irq to be propagated to the IRQ pin
   * For test purposes propagate the IdleIrq and loAlert
   */
  regVal = 0xA0; //rx irq
  mfrc522.PCD_WriteRegister(mfrc522.ComIEnReg, regVal);

  bNewInt = false; //interrupt flag

  /*Activate the interrupt*/
  attachInterrupt(digitalPinToInterrupt(IRQ_PIN), readCard, FALLING);
  attachInterrupt(digitalPinToInterrupt(IRQ_PIN), removedCard, RISING);

  // do { //clear a spourious interrupt at start
  //   ;
  // } while (!bNewInt);
  bNewInt = false;

  Serial.println(F("*****************************"));
  Serial.println(F("Initializing LEDs"));
  FastLED.addLeds<WS2813, DATA_PIN, RGB>(leds, NUM_LEDS);

  Serial.println(F("End setup"));
}

/**
 * Helper routine to dump a byte array as hex values to Serial.
 */
void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}


int DigitLEDs[37];

int Segments[10][7]
 = {
  { 1, 1, 1, 1, 1, 1, 0}, //0
  { 1, 1, 0, 0, 0, 0, 0}, //1
  { 0, 1, 1, 0, 1, 1, 1}, //2
  { 1, 1, 1, 0, 0, 1, 1}, //3
  { 1, 1, 0, 1, 0, 0, 1}, //4
  { 1, 0, 1, 1, 0, 1, 1}, //5
  { 1, 0, 1, 1, 1, 1, 1}, //6
  { 1, 1, 1, 0, 0, 0, 0}, //7
  { 1, 1, 1, 1, 1, 1, 1}, //8
  { 1, 1, 1, 1, 0, 1, 1}, //9
};

int groupSizes[7] = {5, 5, 6, 5, 5, 6, 5};

/**
 * Main loop.
 */
void loop() {
  for (int i = 0; i <220; i++) {
    leds[i] = CRGB::Black;
  }
  FastLED.show();

//int digit = 0;

for (int digit = 0; digit < 10; digit++) {
int index = 0;

for (int i = 0; i < 7; i++) {
    for (int j = 0; j < groupSizes[i]; j++) {
        DigitLEDs[index++] = Segments[digit][i];
    }
}

// Außen 32
// Innen 5
  for (int i = 0; i < 37; i++) {
    leds[i] = CRGB::Green * DigitLEDs[i];
    // if (i >= 10)
    //   leds[i - 10] = CRGB::Black;


    //delay(2000);
  }
      
    FastLED.show();
  delay(1000);
}
  // if (bNewInt) { //new read interrupt
  //   Serial.print(F("Interrupt. "));
  //   mfrc522.PICC_ReadCardSerial(); //read the tag data
  //   // Show some details of the PICC (that is: the tag/card)
  //   Serial.print(F("Card UID:"));
  //   dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  //   Serial.println();

  //   clearInt(mfrc522);
  //   mfrc522.PICC_HaltA();
  //   bNewInt = false;
  // }

  // // The receiving block needs regular retriggering (tell the tag it should transmit??)
  // // (mfrc522.PCD_WriteRegister(mfrc522.FIFODataReg,mfrc522.PICC_CMD_REQA);)
  // activateRec(mfrc522);
  //delay(4000);
} //loop()


/*
 * The function sending to the MFRC522 the needed commands to activate the reception
 */
void activateRec(MFRC522 mfrc522) {
  mfrc522.PCD_WriteRegister(mfrc522.FIFODataReg, mfrc522.PICC_CMD_REQA);
  mfrc522.PCD_WriteRegister(mfrc522.CommandReg, mfrc522.PCD_Transceive);
  mfrc522.PCD_WriteRegister(mfrc522.BitFramingReg, 0x87);
}

/*
 * The function to clear the pending interrupt bits after interrupt serving routine
 */
void clearInt(MFRC522 mfrc522) {
  mfrc522.PCD_WriteRegister(mfrc522.ComIrqReg, 0x7F);
}