#include <SPI.h>
#include <MFRC522.h>
#include <NimBLEDevice.h>

#include "RTClib.h"
#include "SDCardFileAccess.h"
#include "RFIDReader.h"
#include "TimerModule.h"
#include "NeonDisplay.h"
#include "AudioModule.h"

#include <Keypad.h>


// Map RFID UIDs to time spans
std::map<String, TimeSpan> uidToTimeSpan = {
    {"041324D7220289", TimeSpan(0, 0, 1, 0)},
    {"0473A5D7220289", TimeSpan(0, 0, 2, 0)},
    {"045336C6220289", TimeSpan(0, 0, 3, 0)},
    {"0413DAC8220289", TimeSpan(0, 0, 5, 0)},
    {"04D371C6220289", TimeSpan(0, 0, 7, 0)},
    {"047390D6220289", TimeSpan(0, 0, 10, 0)},
    {"0443E3D6220289", TimeSpan(0, 0, 12, 0)},
    {"045398CA220289", TimeSpan(0, 0, 15, 0)},
    {"0493B1D9220289", TimeSpan(0, 0, 20, 0)},
    {"0463C1D7220289", TimeSpan(0, 0, 30, 0)},
    {"0463F1D7220289", TimeSpan(0, 0, 45, 0)},
    {"04137CC8220289", TimeSpan(0, 1, 0, 0)},
    {"04F399C7220289", TimeSpan(0, 1, 15, 0)},
    {"04C3D3C7220289", TimeSpan(0, 1, 30, 0)}
};

const byte ROWS = 5; // four rows
const byte COLS = 4; // three columns
char keys[ROWS][COLS] = {
    {'A', 'B', 'C', 'D'},
    {'E', 'F', 'G', 'H'},
    {'I', 'J', 'K', 'L'},
    {'M', 'N', 'O', 'P'},
    {'Q', 'R', 'S', 'T'}};

byte rowPins[ROWS] = {36, 39, 34, 35, 32}; // connect to the row pinouts of the kpd
byte colPins[COLS] = {4, 17, 2, 33};       // connect to the column pinouts of the kpd

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

byte regVal = 0x7F;

RTC_DS1307 rtc;
SDCardFileAccess sdCardFileAccess;
RFIDReader rfidReader;
TimerModule timerModule;
NeonDisplay neonDisplay;
AudioModule audioModule;

int volume = 1;

BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;

bool timerIsStarted = false;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

/**  None of these are required as they will be handled by the library with defaults. **
 **                       Remove as you see fit for your needs                        */
class MyServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *pServer)
    {
        deviceConnected = true;
        BLEDevice::startAdvertising();
    };

    void onDisconnect(BLEServer *pServer)
    {
        deviceConnected = false;
    }
    /***************** New - Security handled here ********************
    ****** Note: these are the same return values as defaults ********/
    uint32_t onPassKeyRequest()
    {
        Serial.println("Server PassKeyRequest");
        return 123456;
    }

    bool onConfirmPIN(uint32_t pass_key)
    {
        Serial.print("The passkey YES/NO number: ");
        Serial.println(pass_key);
        return true;
    }

    void onAuthenticationComplete(ble_gap_conn_desc desc)
    {
        Serial.println("Starting BLE work!");
    }
    /*******************************************************************/
};

class ConfigCallbacks : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic)
    {
        std::string value = pCharacteristic->getValue();
        // Konfigurationen verarbeiten
        Serial.print("Konfiguration erhalten: ");
        Serial.println(value.c_str());
        // Hier können Konfigurationseinstellungen vorgenommen werden
    }
};

/**
 * Initialize.
 */
void setup()
{
    Serial.begin(115200);
    delay(1000);
    while (!Serial)
        ;

    Serial.println(F("---------------------------------------------"));
    Serial.println(F("Starting setup"));
    Serial.println(F("---------------------------------------------"));
    Serial.println();

    // Serial.println(F("*************************************"));
    // Serial.println(F("Initializing Bluetooth"));
    // Serial.println(F("*************************************"));
    // BLEDevice::init("NeonClock");
    // // Create the BLE Server
    // pServer = BLEDevice::createServer();
    // pServer->setCallbacks(new MyServerCallbacks());

    // // Create the BLE Service
    // BLEService *pService = pServer->createService(SERVICE_UUID);

    // // Create a BLE Characteristic
    // pCharacteristic = pService->createCharacteristic(
    //                     CHARACTERISTIC_UUID,
    //                     NIMBLE_PROPERTY::READ   |
    //                     NIMBLE_PROPERTY::WRITE  |
    //                     NIMBLE_PROPERTY::NOTIFY |
    //                     NIMBLE_PROPERTY::INDICATE
    //                   );
    // pCharacteristic->setCallbacks(new ConfigCallbacks());

    // // Start the service
    // pService->start();

    // // Start advertising
    // BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    // pAdvertising->addServiceUUID(SERVICE_UUID);
    // pAdvertising->setScanResponse(false);
    // /** Note, this could be left out as that is the default value */
    // pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter

    // BLEDevice::startAdvertising();

    Serial.println(F("*************************************"));
    Serial.println(F("Initializing and testing RTC Module"));
    Serial.println(F("*************************************"));
    timerModule.init();
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

    Serial.println(F("*************************************"));
    Serial.println(F("Initializing LEDs"));
    Serial.println(F("*************************************"));
    neonDisplay.Init();
    Serial.println("Done initializing LEDs");
    Serial.println("");

    Serial.println(F("*************************************"));
    Serial.println(F("Testing Audio"));
    Serial.println(F("*************************************"));

    // audio.connecttoSD("intro.wav");

    // while (audio.isRunning()) {
    //    audio.loop();
    // }

    Serial.println(F("End setup"));
}

TimeSpan duration = TimeSpan(0, 0, 0, 0);
char digits[] = {'0', '1', '2', '3', '4'};

void updateDigitsWithCurrentTime(char digits[])
{
    // DateTime timeinfo = rtcModule.getCurrentTime();

    // int minutes = timeinfo.minute();
    // int seconds = timeinfo.second();

    time_t now = time(nullptr);
    struct tm *timeinfo = localtime(&now);

    int minutes = timeinfo->tm_min;
    int seconds = timeinfo->tm_sec;
    int milliseconds = millis() % 1000;

    digits[0] = '0' + (minutes / 10);
    digits[1] = '0' + (minutes % 10);
    digits[2] = '0' + (milliseconds / 100);
    digits[3] = '0' + (seconds / 10);
    digits[4] = '0' + (seconds % 10);

    // if (digits[2] == ' ')
    //   digits[2] = ':';
    // else
    //   digits[2] = ' ';
}

void updateDigitsFromTime(char digits[], TimeSpan time)
{
    int minutes = time.minutes() + time.hours() * 60;
    int seconds = time.seconds();

    digits[0] = '0' + (minutes / 10);
    digits[1] = '0' + (minutes % 10);
    digits[2] = ':';
    digits[3] = '0' + (seconds / 10);
    digits[4] = '0' + (seconds % 10);
}

/**
 * Main loop.
 */
void loop()
{
    bool durationIsUpdated = false;
    char key = keypad.getKey();

    if (key)
    {
        Serial.println(key);
        if (key == 'A')
        {
            duration = timerModule.getRemainingTime() + TimeSpan(0, 0, 1, 0);
            timerModule.startTimer(duration);
            durationIsUpdated = true;
        }
        if (key == 'E')
        {
            duration = timerModule.getRemainingTime() + TimeSpan(0, 0, -1, 0);
            timerModule.startTimer(duration);
            durationIsUpdated = true;
        }
        if (key == 'C' || key == 'D' || key == 'G' || key == 'H')
        {

            audioModule.playAudio("Start.wav", volume);
            timerModule.startTimer(duration);
            timerIsStarted = true;
        }
        if (key == 'T')
        {
            duration = TimeSpan(0, 0, 0, 0);
            timerModule.stopTimer();
            durationIsUpdated = true;
            timerIsStarted = false;
        }
    }

    if(timerModule.isTimerRunning()) {
        updateDigitsFromTime(digits, timerModule.getRemainingTime());
        neonDisplay.Show(digits);
    }
    else if (timerIsStarted) {
        neonDisplay.Blink();
    }

    // updateDigitsWithCurrentTime(digits);
    // Serial.println(digits);
    //neonDisplay.Show(digits);

    // // notify changed value
    // if (deviceConnected) {
    //     pCharacteristic->setValue((uint8_t*)&value, 4);
    //     pCharacteristic->notify();
    //     value++;
    //     delay(10); // bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
    // }
    // // disconnecting
    // if (!deviceConnected && oldDeviceConnected) {
    //     delay(500); // give the bluetooth stack the chance to get things ready
    //     pServer->startAdvertising(); // restart advertising
    //     Serial.println("start advertising");
    //     oldDeviceConnected = deviceConnected;
    // }
    // // connecting
    // if (deviceConnected && !oldDeviceConnected) {
    //     // do stuff here on connecting
    //     oldDeviceConnected = deviceConnected;
    // }
    // for (int i = 0; i <220; i++) {
    //   leds[i] = CRGB::Black;
    // }
    // FastLED.show();

    // if (mp3->isRunning()) {
    //   if (!mp3->loop()) mp3->stop();
    // }

    // audio.loop();

    String uid = rfidReader.readCard();

    if (uid != "")
    {
        if (uidToTimeSpan.find(uid) != uidToTimeSpan.end()) {
            duration = uidToTimeSpan[uid];
            durationIsUpdated = true;

            audioModule.playAudioAsync("confirm.wav", volume);     
        }
    }

    if (durationIsUpdated)
    {
        timerIsStarted = false;
        updateDigitsFromTime(digits, duration);
        neonDisplay.Show(digits);
    }
}
