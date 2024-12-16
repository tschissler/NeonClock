#include "TimerModule.h"
#include <Arduino.h> 
#include <Wire.h>

TimerModule::TimerModule() : rtc() {}

void TimerModule::init() {
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

void TimerModule::startTimer(TimeSpan duration) {
    time_t now = time(nullptr);
    endTime = now + duration.totalseconds();
    Serial.print("Timer started - Start time: ");
    Serial.print(now);
    Serial.print(", End time: ");
    Serial.println(endTime);
}

void TimerModule::stopTimer() {
    endTime = 0;
}

TimeSpan TimerModule::getRemainingTime() {
    time_t now = time(nullptr);
    time_t remainingSeconds = endTime - now;

    int hours = remainingSeconds / 3600;
    remainingSeconds %= 3600;
    int minutes = remainingSeconds / 60;
    int seconds = remainingSeconds % 60;

    return TimeSpan(0, hours, minutes, seconds);
}

DateTime TimerModule::getCurrentTime() {
    return rtc.now();
}

bool TimerModule::isTimerRunning() {
    time_t now = time(nullptr);
    return now <= endTime;
}