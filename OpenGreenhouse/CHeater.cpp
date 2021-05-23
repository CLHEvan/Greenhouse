#include "CHeater.h"

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
    
    digitalWrite(pin, result);
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
