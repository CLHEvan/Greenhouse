#ifndef STIME_H
#define STIME_H
/*
 * is based on RTClib: 
 *     https://github.com/adafruit/RTClib
 * 
 * this class still make an abstraction with other classes and RTClib to avoid dependancies
 * 
 */

#include <Arduino.h>
#include <RTClib.h>
#include "OGConfig.h"

typedef RTC_DS3231 RTC;

struct STimeYear
{
    STimeYear
        (uint16_t year, uint8_t month, uint8_t day, uint8_t hour,
         uint8_t minute, uint8_t second) : year(year), month(month), 
         hour(hour), minute(minute), second(second) {}
    STimeYear() : year(0), month(0), 
         hour(0), minute(0), second(0) {}
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;

    bool operator==(const STimeYear& cmp)
    {
        return cmp.year == year && cmp.month == month && cmp.day == day &&
                cmp.hour == hour && cmp.minute == minute && cmp.second == second;
    }
    bool operator!=(const STimeYear& cmp) { return !(*this==cmp); }
};

class STime : IConfigurable
{
public:
    STime();
    
    bool bewteenHours(uint8_t h1, uint8_t h2, uint8_t cmp);
    
    STimeYear getTime();
    STimeYear getLastUpdatedTime();
    
    void update();
    void begin();

    OGName getConfigName() override;
    SParameter* getParams(int& length) override;
    
private:
    RTC rtc;
    STimeYear lastUpdateTime;

    void changeTime(STimeYear time);

    //adjustTime is use to change time by paramter, lastAdjustedTime is use to detect when time has been changed by diference
    STimeYear adjustTime;
    STimeYear lastAdjustedTime;

    SParameter params[1] = {
        SParameter(
            OGName("adj. time", 9),
            SType(EType::STRUCT_T, sizeof(STimeYear)),
            &adjustTime,
            false
        )
    };
};

#endif
