#ifndef CHEATER_H
#define CHEATER_H

#include <Arduino.h>
#include "OGConfig.h"
#include "STemp.h"

class CHeater : public IConfigurable
{
public:
    CHeater(STemp* tempSensorn, uint8_t pin, double temp, double period)
                : tempSensor(tempSensor), ptemp(temp), pperiod(period)
    {
        pinMode(pin, OUTPUT);
    }
    
    void update();
    
    OGName getConfigName() override;

    SParameter* getParams(int& length) override;
    
private:
    STemp* tempSensor;
    uint8_t pin;
    
    double ptemp;   //parameter temperature
    double pperiod; //parameter period
    
    bool isActive = false;

    //parameters
    SParameter params[2] =
    {
        SParameter (
            OGName("Temperature", 11),
            SType(EType::FLOAT_T, sizeof(double)),
            &ptemp
        ), 
        SParameter (
            OGName("Period", 6),
            SType(EType::FLOAT_T, sizeof(double)),
            &pperiod
        )
    };
};

#endif
