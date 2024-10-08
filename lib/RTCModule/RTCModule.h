#ifndef RTCMODULE_H
#define RTCMODULE_H

#include <RTClib.h>

class RTCModule {
public:
    RTCModule();
    void init();
    DateTime getCurrentTime();

private:
    RTC_DS1307 rtc;
};

#endif // RTCMODULE_H