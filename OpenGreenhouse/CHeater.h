#ifndef HEATER_H
#define HEATER_H

#include "Arduino.h"
#include "ITemp.h"

/*
 * CHeater class
 * 
 * manage heater pin with a PID and ITemp class
 * 
 */
class CHeater
{
public:
  CHeater(ITemp* temp, uint8_t heater);
  
  void update();
  
  void setActive(bool act);
  bool getActive();

  void setKp(double Kp);
  void setKi(double Ki);
  void setKd(double Kd);
  double getKp();
  double getKi();
  double getKd();
  
private:
  bool _active = false;
  
  ITemp* _temp;
  uint8_t _heater;
  
  double _Kp;
  double _Ki;
  double _Kd;
};

#endif
