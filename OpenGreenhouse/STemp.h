#ifndef STEMP_H
#define STEMP_H

#include <Arduino.h>

/*
 * temprature provider, 
 * based on LM35.
 * 
 */
class STemp
{
public:
    STemp(uint8_t spin);

    //temperature in celcuis
    double getTemp();
    
private:
    uint8_t sensorPin;
};

#endif
