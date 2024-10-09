#include "RFIDReader.h"
#include <Arduino.h> // Include Arduino for Serial

MFRC522 mfrc522(SDA_PIN, RST_PIN);   // Create MFRC522 instance.
MFRC522::MIFARE_Key key;

volatile bool bNewInt = false;

/**
 * MFRC522 interrupt serving routine
 */
void RFIDReader::readTag() {
  bNewInt = true;
  Serial.println(F("Interrupt routine"));
}

void RFIDReader::removedTag() {
  bNewInt = true;
  Serial.println(F("Tag removed"));
}

void RFIDReader::init() {
    SPI.begin(SCLK_PIN, MISO_PIN, MOSI_PIN, SDA_PIN);
    mfrc522.PCD_Init();
    mfrc522.PCD_DumpVersionToSerial();  // Show version of PCD - MFRC522 Card Reader
    Serial.println(F("Performing test..."));
    bool result = mfrc522.PCD_PerformSelfTest(); // perform the test
    Serial.println(F("-----------------------------"));
    Serial.print(F("Result: "));
    if (result)
        Serial.println(F("OK"));
    else
        Serial.println(F("DEFECT or UNKNOWN"));
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

    /*Activate the interrupt*/
    //attachInterrupt(digitalPinToInterrupt(IRQ_PIN), std::bind(&RFIDReader::readTag, this), FALLING);
    //attachInterrupt(digitalPinToInterrupt(IRQ_PIN), std::bind(&RFIDReader::removedTag, this), RISING);

    bNewInt = false; //interrupt flag
}

/**
 * Helper routine to dump a byte array as hex values to Serial.
 */
String dump_byte_array(byte *buffer, byte bufferSize) {
    std::string result;
    for (byte i = 0; i < bufferSize; i++) {
        char hex[3]; // Two hex digits and a null terminator
        sprintf(hex, "%02X", buffer[i]);
        result += hex;
    }
    return result.c_str();
}

String RFIDReader::readCard() {

    //Serial.println(F("Reading card..."));

	// Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
	if ( ! mfrc522.PICC_IsNewCardPresent()) {
		return "";
	}

	// Select one of the cards
	if ( ! mfrc522.PICC_ReadCardSerial()) {
		return "";
	}

	// Dump debug info about the card; PICC_HaltA() is automatically called
  String uidStr = dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  Serial.println(uidStr.c_str());
  mfrc522.PICC_HaltA();
	//mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
  return uidStr;




    // Serial.println(F(bNewInt ? "New interrupt" : "No new interrupt"));

    // if (bNewInt) { //new read interrupt
    //     Serial.print(F("Interrupt. "));
    //     mfrc522.PICC_ReadCardSerial(); //read the tag data
    //     // Show some details of the PICC (that is: the tag/card)
    //     Serial.print(F("Card UID:"));
    //     dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    //     Serial.println();

    //     clearInt();
    //     mfrc522.PICC_HaltA();
    //     bNewInt = false;
    // }

    // // The receiving block needs regular retriggering (tell the tag it should transmit??)
    // // (mfrc522.PCD_WriteRegister(mfrc522.FIFODataReg,mfrc522.PICC_CMD_REQA);)
    // activateRec();

    // // Add code to read card
    // if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    //      return;
    // }
    // Serial.print("Tag UID:");
    // for (byte i = 0; i < mfrc522.uid.size; i++) {
    //     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    //     Serial.print(mfrc522.uid.uidByte[i], HEX);
    // }
    // Serial.println();
    // mfrc522.PICC_HaltA();
}


