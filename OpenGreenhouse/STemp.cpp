#include "STemp.h"

STemp::STemp(uint8_t spin) : sensorPin(spin) {}

double STemp::getTemp()
{
    int pinValue = analogRead(sensorPin);
    
    return (double)pinValue * 100.0 / 1023.0;
}
