#ifndef CHEATER_H
#define CHEATER_H

#include "Arduino.h"
#include "ITemp.h"

class CHeater
{
public:
  CHeater(ITemp* temp, uint8_t pin);

  void update();

  void setTemp(double on, double off);
  double getOn();
  double getOff();

  void setActive(bool state);
  bool getActive();

private:
  bool _active = false;
  uint8_t _heaterPin;
  ITemp* _temp;

  double _setOn = 23;
  double _setOff = 25;
};

#endif
