#include "CHeater.h"

CHeater::CHeater(ITemp* temp, uint8_t heater) : _temp(temp), _heater(heater)
{
  pinMode(heater, OUTPUT);
}

void CHeater::update()
{
  if(!_active) return;
}

void CHeater::setActive(bool act) { this->_active = act; }
bool CHeater::getActive() { return this->_active; }

void CHeater::setKp(double Kp)
{
  _Kp = Kp;
}
void CHeater::setKi(double Ki)
{
  _Ki = Ki;
}
void CHeater::setKd(double Kd)
{
  _Kd = Kd;
}
double CHeater::getKp()
{
  return _Kp;
}
double CHeater::getKi()
{
  return _Ki;
}
double CHeater::getKd()
{
  return _Kd;
}
