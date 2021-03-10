#include "CLight.h"

CLight::CLight(ITime* time, uint8_t light) : _time(time), _light(light)
{
  pinMode(light, OUTPUT);
}

void CLight::update()
{
  if(!_active) return;
  
  bool state = this->_time->betweenHours(this->_startHour, this->_stopHour);
  if(state == this->_state) return;
  
  this->_state = state;
  digitalWrite(this->_light, state);
}

void CLight::setCycle(uint8_t startHour, uint8_t stopHour)
{
  this->_startHour = startHour;
  this->_stopHour  = stopHour;
}

uint8_t CLight::getStartHour() { return _startHour; }
uint8_t CLight::getStopHour()  { return _stopHour;  }

void CLight::setActive(bool act) { this->_active = act; }
bool CLight::getActive() { return this->_active; }
