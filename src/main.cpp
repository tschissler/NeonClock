#include <SPI.h>
#include <MFRC522.h>
#include <FastLED.h>
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2SNoDAC.h"
#include "AudioFileSourceSD.h"


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

AudioGeneratorMP3 *mp3;
AudioFileSourceSD *file;
AudioOutputI2S *out;
AudioFileSourceID3 *id3;

// Called when a metadata event occurs (i.e. an ID3 tag, an ICY block, etc.
void MDCallback(void *cbData, const char *type, bool isUnicode, const char *string)
{
  (void)cbData;
  Serial.printf("ID3 callback for: %s = '", type);

  if (isUnicode) {
    string += 2;
  }
  
  while (*string) {
    char a = *(string++);
    if (isUnicode) {
      string++;
    }
    Serial.printf("%c", a);
  }
  Serial.printf("'\n");
  Serial.flush();
}
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

  // Serial.println(F("*****************************"));
  // Serial.println(F("Initializing and testing Audio"));
  // audioLogger = &Serial;
  // file = new AudioFileSourceSD("/intro.mp3");
  // id3 = new AudioFileSourceID3(file);
  // id3->RegisterMetadataCB(MDCallback, (void*)"ID3TAG");
  // out = new AudioOutputI2S(0,AudioOutputI2S::INTERNAL_DAC);
  // mp3 = new AudioGeneratorMP3();
  // mp3->begin(id3, out);

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
int digit = 0;
/**
 * Main loop.
 */
void loop() {
//   for (int i = 0; i <220; i++) {
//     leds[i] = CRGB::Black;
//   }
//   FastLED.show();

  // if (mp3->isRunning()) {
  //   if (!mp3->loop()) mp3->stop();
  // } 

  // return;
  String uid = rfidReader.readCard();

  if (uid == "0453FDC8220289")
    digit = 1;
  if (uid == "048350D9220289")
    digit = 2;
  if (uid == "04A3AECD220289")
    digit = 3;


//for (int digit = 0; digit < 10; digit++) {
int index = 0;

for (int i = 0; i < 7; i++) {
    for (int j = 0; j < groupSizes[i]; j++) {
        DigitLEDs[index++] = Segments[digit][i];
    }
  }

// // Außen 32
// // Innen 5
  for (int i = 0; i < 37; i++) {
    leds[i] = CRGB::Green * DigitLEDs[i];
    // if (i >= 10)
    //   leds[i - 10] = CRGB::Black;


    //delay(2000);
  }
    FastLED.show();
//}

//   rfidReader.readCard();
   delay(100);


  // int index = 0;

  // for (int i = 0; i < 7; i++) {
  //     for (int j = 0; j < groupSizes[i]; j++) {
  //         DigitLEDs[index++] = Segments[digit][i];
  //     }
  // }
  // FastLED.show();

} //loop()
