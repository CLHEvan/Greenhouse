#ifndef BUFFER_H
#define BUFFER_H

#include <Arduino.h>

/*
 * Made this class to simply read data buffer and binary struct
 */
class Buffer
{
public:
    Buffer(char* buff, size_t length);

    void getVoidValue(size_t size, void* dest);

    void setVoidValue(size_t size, void* value);

    template<typename T>
    T getValue()
    {
        _cursor += sizeof(T);
        return getValue<T>(_cursor);
    }

    template<typename T>
    void setValue(T value)
    {
        setValue<T>(value, _cursor);
        _cursor += sizeof(T);
    }

    template<typename T>
    void setValueFromPointer(T value)
    {
        setValueFromPointer<T>(value, _cursor);
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

    template<typename T>
    void setValueFromPointer(T value, size_t index)
    {
        memcpy(&_buff[index], value, sizeof(T));
    }

    void set(size_t index, char value);
    char get(size_t index);

    void setCursor(size_t cursor);
    size_t getCursor();

private:
    size_t _size;
    char* _buff;

    size_t _cursor = 0u;
};

#endif
