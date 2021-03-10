#ifndef ITIME_H
#define ITIME_H

#include "Arduino.h"

/*
 * ITime class
 * 
 * abstraction for time modules
 * 
 */
class ITime
{
public:
  
  //if current hour is between hour1 and hour2
  virtual bool betweenHours(uint8_t hour1, uint8_t hour2) = 0;
  
};

#endif
