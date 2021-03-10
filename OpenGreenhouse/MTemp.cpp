#include "MTemp.h"

MTemp::MTemp(uint8_t in) : _in(in)
{
  
}

double MTemp::getTemp() 
{
  int read = analogRead(_in);
  double temp = (double) read * (5.0 / 1023.0 * 100.0);
  
  return temp; 
}
