#ifndef OGCONFIG_H
#define OGCONFIG_H

#include <Arduino.h>

#define NAME_SIZE 12

//used by struct SType, change in another int type for greater size than 255(like structs).
//you should use the smallest type possible to save data transmition size
//if you change this, you must verify data alignement of struct SType
typedef char STSize_t;

enum class EType {INT_T, FLOAT_T, STRUCT_T};

//by default max size is 255, constraint by char(typedef char STSize_t;), can be problematic for structs
struct SType
{
    SType(EType type, STSize_t size) : type(type), size(size) {}
    EType type;
    STSize_t size;
};

struct OGName
{
    OGName(const char pname[NAME_SIZE], int length)
    {
        for (int i = 0; i < length && i < NAME_SIZE; i++)
            name[i] = pname[i];
        for (int i = length; i < NAME_SIZE; i++)
            name[i] = 0x00;
    }
    char name[NAME_SIZE];
};

struct SParameter
{
    SParameter(OGName name, SType type, void* value)
                : name(name), type(type), value(value) {}
    OGName name;
    SType type;
    void* value;
};

class IConfigurable
{
public:
    virtual SParameter* getParams(int& length) = 0;
    virtual OGName getConfigName() = 0;
};

class OGConfig
{
public:
    OGConfig(IConfigurable** configurables, int clength)
                : configurables(configurables), configLength(clength)
    {
        //init parameters and defin paramsLength
        paramsLength = 0;
        for(int i = 0; i < clength; i++)
        {
            int length;
            configurables[i]->getParams(length);
            paramsLength += length;
        }
    }

    char* onCommand(char* cmd, int clength, int& rlength);

private:
    IConfigurable** configurables;
    int configLength; //configurables length
    int paramsLength; //all parameters length of configurables
};

#endif
