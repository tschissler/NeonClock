#include <SPI.h>
#include <MFRC522.h>
#include <FastLED.h>
#include "RTClib.h"
#include "SDCardFileAccess.h" 
#include "RFIDReader.h"
#include "RTCModule.h"

#define NUM_LEDS 180
#define DATA_PIN 16

// Define the array of leds
CRGB leds[NUM_LEDS];

byte regVal = 0x7F;

RTC_DS1307 rtc;
SDCardFileAccess sdCardFileAccess; 
RFIDReader rfidReader;
RTCModule rtcModule;

/**
 * Initialize.
 */
void setup() {
  Serial.begin(115200); 
  while (!Serial);   
  
  Serial.println(F("---------------------------------------------"));
  Serial.println(F("Starting setup"));
  Serial.println(F("---------------------------------------------"));
  Serial.println();

  Serial.println(F("*************************************"));
  Serial.println(F("Initializing and testing RTC Module"));
  Serial.println(F("*************************************"));
  rtcModule.init();
  Serial.println("");

  Serial.println(F("*************************************"));
  Serial.println(F("Initializing and testing SD card"));
  Serial.println(F("*************************************"));
  sdCardFileAccess.init();
  Serial.println("");

  Serial.println(F("*************************************"));
  Serial.println(F("Initializing and testing RFID reader"));
  Serial.println(F("*************************************"));
  rfidReader.init();
  Serial.println("");

  Serial.println(F("*****************************"));
  Serial.println(F("Initializing LEDs"));
  FastLED.addLeds<WS2813, DATA_PIN, RGB>(leds, NUM_LEDS);

  Serial.println(F("End setup"));
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
//   for (int i = 0; i <220; i++) {
//     leds[i] = CRGB::Black;
//   }
//   FastLED.show();

// //int digit = 0;

// for (int digit = 0; digit < 10; digit++) {
// int index = 0;

// for (int i = 0; i < 7; i++) {
//     for (int j = 0; j < groupSizes[i]; j++) {
//         DigitLEDs[index++] = Segments[digit][i];
//     }
// }

// // Außen 32
// // Innen 5
//   for (int i = 0; i < 37; i++) {
//     leds[i] = CRGB::Green * DigitLEDs[i];
//     // if (i >= 10)
//     //   leds[i - 10] = CRGB::Black;


//     //delay(2000);
//   }
//     FastLED.show();
//   delay(1000);
// }

//   rfidReader.readCard();
   delay(100);

  rfidReader.readCard();

} //loop()
