#ifndef MTIME_H
#define MTIME_H

#include "ITime.h"

class MTime : public ITime
{
public:
  MTime();

  virtual bool betweenHours(uint8_t hour1, uint8_t hour2) override;
};

#endif
