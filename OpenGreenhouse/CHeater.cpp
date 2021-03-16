#include "CHeater.h"

CHeater::CHeater(ITemp* temp, uint8_t pin) : _temp(temp), _heaterPin(pin)
{
  pinMode(pin, OUTPUT);
}

void CHeater::update()
{
  if(!_active) return;

  double temp = _temp->getTemp();

  if(temp < _setOn)
    digitalWrite(_heaterPin, HIGH);
  else if(temp > _setOff)
    digitalWrite(_heaterPin, LOW);
  /*else do nothing*/
}


void CHeater::setTemp(double on, double off)
{
  _setOn  = on;
  _setOff = off;
}

double CHeater::getOn()
{
  return _setOn;
}
double CHeater::getOff()
{
  return _setOff;
}

void CHeater::setActive(bool state)
{
  _active = state;

  if(!state) digitalWrite(_heaterPin, LOW);
}

bool CHeater::getActive()
{
  return _active;
}
