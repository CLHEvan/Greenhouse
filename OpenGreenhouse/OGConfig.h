#ifndef OGCONFIG_H
#define OGCONFIG_H

#include <Arduino.h>

#define NAME_SIZE 12

#define CMD_GET_ALL         0x01 //cmd = {name: GetAll       } , {return: <cmd>, [<id_config>, <id_param>, <type>, <size>, <value>] } , {param:                                  }
#define CMD_SET_PARAM       0x02 //cmd = {name: SetParam     } , {return: <cmd>, <id_config>, <id_param>, <type>, <size>, <value>   } , {param: <id_config>, <id_param>, <value> }
#define CMD_BLIND_SET_PARAM 0x03 //cmd = {name: SetParam     } , {return:                                                           } , {param: <id_config>, <id_param>, <value> }
#define CMD_GET_PARAM       0x04 //cmd = {name: GetParam     } , {return: <cmd>, <id_config>, <id_param>, <type>, <size>, <value>   } , {param: <id_config>, <id_param>          }
#define CMD_SAVE            0x05 //cmd = {name: SaveParams   } , {return:                                                           } , {param:                                  }
#define CMD_READ            0x06 //cmd = {name: ReadParams   } , {return:                                                           } , {param:                                  }
#define CMD_GET_SAVABLES    0x07 //cmd = {name: GetSaveables } , {return: <cmd>, [<id_config>, <id_param>, <type>, <size>, <value>] } , {param:                                  }

//used by struct SType, change in another int type for greater size than 255(like structs).
//you should use the smallest type possible to save memory.
//if you change this, you must verify data alignement of struct SType.
typedef char STSize_t;

enum class EType:char{INT_T = 0x01, FLOAT_T = 0x02, STRUCT_T = 0x03};

//containe type informations
struct SType
{
    SType(EType type, STSize_t size) : type(type), size(size) {}
    EType type;
    STSize_t size;
};

//contain name used to identify paramters and configurables
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
                : name(name), type(type), value(value), saveable(true) {}
    SParameter(OGName name, SType type, void* value, bool save)
                : name(name), type(type), value(value), saveable(save) {}
    OGName name;
    SType type;
    void* value;
    bool saveable; //if ConfigSaver can save it
};

class IConfigurable
{
public:
    virtual SParameter* getParams(int& length) = 0;
    virtual OGName getConfigName() = 0;
};

class OGConfig; //forward declaration (codepends between OGConfig and ConfigSaver)

/*
 * Class who save parameters.
 * define in ConfigSaver.cpp
 */
class ConfigSaver
{
public:
    ConfigSaver(OGConfig* config, uint8_t sdpin);

    //return false if an error has occurred esle return true
    bool saveConfig();
    bool readConfig();
    //

private:
    OGConfig* config;
};

class OGConfig
{
public:
    OGConfig(IConfigurable** configurables, int clength, uint8_t sdpin);
    ~OGConfig();

    //interpreter of commands
    void  onCommand(char* cmd, int clength);
    char* onCommand(char* cmd, int clength, int& rlength); //the buffer response must be deleted
    //

private:
    ConfigSaver* saver;  //object to save saveable configurables
    IConfigurable** configurables;
    int configLength;    //configurables length
    int paramsLength;    //all parameters length of configurables
    int saveablesLength; //saveable configurables length
};

#endif
