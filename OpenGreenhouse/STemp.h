#ifndef STEMP_H
#define STEMP_H

#include <Arduino.h>

/*
 * based on LM35.
 */
class STemp
{
public:
    STemp(uint8_t spin);

    //Get temperature in celcuis
    double getTemp();
    
private:
    uint8_t sensorPin;
};

#endif
