#include "Buffer.h"

Buffer::Buffer(byte* buff, size_t size) : _buff(buff), _size(size) {}

void Buffer::setCursor(size_t cursor)
{
  _cursor = cursor;
}

size_t Buffer::getCursor(size_t cursor)
{
  return _cursor;
}
