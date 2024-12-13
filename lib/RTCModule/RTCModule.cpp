#include "RTCModule.h"
#include <Arduino.h> 
#include <Wire.h>

RTCModule::RTCModule() : rtc() {}

void RTCModule::init() {
    if (!rtc.begin()) {
        Serial.println("Couldn't find RTC");
    } else {
        Serial.println("RTC module found");
        if (!rtc.isrunning()) {
            Serial.println("RTC is NOT running, let's set the time!");
            rtc.adjust(DateTime(2024, 10, 8, 9, 12, 0));
        }
        Serial.print("RTC is running - Current time: ");
        Serial.println(rtc.now().timestamp(DateTime::TIMESTAMP_FULL));
    }
}

DateTime RTCModule::getCurrentTime() {
    return rtc.now();
}