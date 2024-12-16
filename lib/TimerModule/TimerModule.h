#ifndef TIMERMODULE_H
#define TIMERMODULE_H

#include <RTClib.h>

class TimerModule {
public:
    TimerModule();
    void init();
    DateTime getCurrentTime();
    void startTimer(TimeSpan duration);
    TimeSpan getRemainingTime();

    bool isTimerRunning();
    
private:
    RTC_DS1307 rtc;
    time_t endTime;
};

#endif // TIMERMODULE_H