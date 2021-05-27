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
        Serial.println("ERROR: Couldn't find RTC");
        Serial.flush();
        abort();
    }
}

bool STime::bewteenHours(uint8_t h1, uint8_t h2, uint8_t cmp)
{
    return h1 <= cmp && h2 >= cmp;
}
bool STime::bewteenHours(uint8_t h1, uint8_t h2)
{
    return this->bewteenHours(h1, h2, lastUpdateTime.hour);
}

OGName STime::getConfigName()
{
    return OGName("Time", 4);
}

SParameter* STime::getParams(int& length)
{
    adjustTime = getLastUpdatedTime();
    lastAdjustedTime = adjustTime;
    
    length = 1;
    return this->params;
}
