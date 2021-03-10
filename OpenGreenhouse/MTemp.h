#ifndef MTEMP_H
#define MTEMP_H

#include "arduino.h"
#include "ITemp.h"

/*
 * MTemp class
 * 
 * provide temperature by a LM35 sensor
 * 
 */
class MTemp : public ITemp
{
public: //TODO: aref pin constructor
  MTemp(uint8_t in);
  
  virtual double getTemp() override;
  
private:
  uint8_t _in;
};

#endif
