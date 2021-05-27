#include "CHeater.h"

CHeater::CHeater(STemp* tempSensorn, uint8_t hpin, double temp, double period)
    : tempSensor(tempSensor), hpin(hpin), ptemp(temp), pperiod(period)
{
    pinMode(hpin, OUTPUT);
}

void CHeater::update()
{
    double temp = this->tempSensor->getTemp();
    
    bool result;
    
    if(isActive)
    {
        if(temp >= (ptemp + pperiod))
            result = false;
        else
            result = true;
    }
    else
    {
        if(temp <= (ptemp - pperiod))
            result = true;
        else
            result = false;
    }

    if(isActive == result)
        return;
    
    isActive = result;
    
    digitalWrite(hpin, result);
}

OGName CHeater::getConfigName()
{
    return OGName("Heater", 6);
}

SParameter* CHeater::getParams(int& length)
{
    length = 2;
    return this->params;
}
