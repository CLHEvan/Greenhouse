#ifndef ITEMP_H
#define ITEMP_H

/*
 * ITemp class
 * 
 * abstraction for temperature sensors
 * 
 */
class ITemp
{
public:
  virtual double getTemp() = 0;
};

#endif
