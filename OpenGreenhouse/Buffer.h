#ifndef BUFFER_H
#define BUFFER_H

#include "Arduino.h"

/*
 * Utility class to manage byte buffers and convert type to byte* or byte* to type
 */
class Buffer
{

public:
  Buffer(byte* buff, size_t length);


  template<typename T> 
  T getValue()
  {
    _cursor -= sizeof(T);
    return getValue<T>(_cursor);
  }

  template<typename T>
  void setValue(T value)
  {
    setValue<T>(value, _cursor);
    _cursor += sizeof(T);
  }

  template<typename T>
  T getValue(size_t index)
  {
    T value;
    memcpy(&value, &_buff[index], sizeof(T));
    return value;
  }

  template<typename T>
  void setValue(T value, size_t index)
  {
    memcpy(&_buff[index], &value, sizeof(T));
  }

  void setCursor(size_t cursor);
  size_t getCursor(size_t cursor);

private:
  size_t _size;
  byte* _buff;

  size_t _cursor = 0u;
};

#endif
