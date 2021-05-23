#ifndef CLIGHT_H
#define CLIGHT_H

#include <Arduino.h>
#include "OGConfig.h"
#include "STime.h"

class CLight : IConfigurable
{
public:
    CLight(STime* time, uint8_t startHour, uint8_t stopHour);

    void update();

    OGName getConfigName() override;
    SParameter* getParams(int& length) override;
    
private:
    uint8_t startHour;
    uint8_t stopHour;

    STime* time;

    SParameter params[2] = 
    {
        SParameter(
            OGName("start", 5),
            SType(EType::INT_T, sizeof(uint8_t)),
            &startHour
        ),
        SParameter(
            OGName("stop", 4),
            SType(EType::INT_T, sizeof(uint8_t)),
            &stopHour
        )
    };
};

#endif
