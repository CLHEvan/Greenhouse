#include "Buffer.h"

Buffer::Buffer(char* buff, size_t size) : _buff(buff), _size(size) {}

void Buffer::getVoidValue(size_t size, void* dest)
{
    _cursor -= size;
    memcpy(&_buff[_cursor], dest, size);
}

void Buffer::setVoidValue(size_t size, void* value)
{
    memcpy(&_buff[_cursor], value, size);
    _cursor += size;
}

void Buffer::set(size_t index, char value)
{
    _buff[index] = value;
}

char Buffer::get(size_t index)
{
    return _buff[index];
}

void Buffer::setCursor(size_t cursor)
{
    _cursor = cursor;
}
size_t Buffer::getCursor(size_t cursor)
{
    return _cursor;
}
