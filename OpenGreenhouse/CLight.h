#ifndef CLIGHT_H
#define CLIGHT_H

#include "Arduino.h"
#include "ITime.h"

/*
 * CLight class
 * 
 * manage light pin with ITime class
 */
class CLight
{
public:
  CLight(ITime* time, uint8_t light); //use read()
  void update();
  
  void setCycle(uint8_t startHour, uint8_t stopHour); //change and save
  
  void setActive(bool act);
  bool getActive();
  
  uint8_t getStartHour();
  uint8_t getStopHour();
  
private:
  bool _active = false;
  
  ITime* _time;
  bool   _state;
  
  uint8_t _light;

  uint8_t _startHour = 0; //enlever les valeurs par defaut
  uint8_t _stopHour  = 18;
};

#endif
