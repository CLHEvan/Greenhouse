#include "STime.h"

STime::STime() {}

STimeYear STime::getTime()
{
    DateTime now = rtc.now();

    STimeYear time (
        now.year(),
        now.month(),
        now.day(),
        now.hour(),
        now.minute(),
        now.second()
    );

    return time;
}

STimeYear STime::getLastUpdatedTime()
{
    return lastUpdateTime;
}

void STime::changeTime(STimeYear time)
{
    rtc.adjust
    (
        DateTime
        (
            time.year,
            time.month,
            time.day,
            time.hour,
            time.minute,
            time.second
        )
    );
}

void STime::update()
{
    
    if(adjustTime != lastAdjustedTime)
    {
        this->changeTime(adjustTime);
        lastAdjustedTime = adjustTime;
    }
    
    lastUpdateTime = getTime();
}

void STime::begin()
{
    if(!rtc.begin())
    {
        Serial.println("Couldn't fin RTC");
        Serial.flush();
        abort();
    }
}

OGName STime::getConfigName()
{
    return OGName("Time", 4);
}

SParameter* STime::getParams(int& length)
{
    length = 1;
    return this->params;
}
