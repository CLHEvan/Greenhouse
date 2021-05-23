#include "CLight.h"

CLight::CLight(STime* time, uint8_t startHour, uint8_t stopHour)
    : time(time), startHour(startHour), stopHour(stopHour) {}


void CLight::update()
{
    
}

OGName CLight::getConfigName()
{
    return OGName("Light", 5);
}

SParameter* CLight::getParams(int& length)
{
    length = 2;
    return params;
}
