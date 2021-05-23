#include "OGConfig.h"
#include "Buffer.h"

#define CMD_GET_ALL   0x01 //cmd = {name: GetAll   } , {return: [<id>, <type>, <value>]                  } , {param: null                             }
#define CMD_SET_PARAM 0x02 //cmd = {name: SetParam } , {return: <id_config>, <id_param>, <type>, <value> } , {param: <id_config>, <id_param>, <value> }
#define CMD_GET_PARAM 0x03 //cmd = {name: GetParam } , {return: <id_config>, <id_param>, <type>, <value> } , {param: <id_config>, <id_param>          }

char* OGConfig::onCommand(char* cmd, int clength, int& rlength)
{
    rlength = 0;
    char* response = nullptr;
    
    if(clength == 0)
        return response;
    
    char cmdName = cmd[0];
    
    if(cmdName == CMD_GET_PARAM || cmdName == CMD_SET_PARAM)
    {
        char cname[NAME_SIZE];
        char pname[NAME_SIZE];
        
        int plength = 0;
        SParameter* sparams = nullptr;
        
        SParameter* cparam = nullptr;
        
        int i;
        for(i = 0; i < NAME_SIZE; i++)
        {
            cname[i] = cmd[i + 1];
        }
        for(i = 0; i < NAME_SIZE; i++)
        {
            pname[i] = cmd[i + NAME_SIZE + 1];
        }
        cname[NAME_SIZE - 1] = 0x00; //make sure char string have null char
        pname[NAME_SIZE - 1] = 0x00;

        for(i = 0; i < this->configLength; i++) //search configurable and take her params
        {
            IConfigurable* conf = this->configurables[i];
            if(strcmp(cname, conf->getConfigName().name) == 0)
            {
                sparams = conf->getParams(plength);
            }
        }

        if(sparams == nullptr) //if not find configurable and/or her params
        {
            return response;
        }

        for(i = 0; i < plength; i++) //searche target param and set cparam
        {
            SParameter param = sparams[i];
            if(strcmp(pname, param.name.name) == 0)
            {
                cparam = &param;
            }
        }

        if(cparam == nullptr) //if not find param
        {
            return response;
        }

        if(cmdName == CMD_SET_PARAM) //set new param value
        {
            SType stype = cparam->type;

            //avoid memory violation
            if(stype.size != (clength - 1 - NAME_SIZE * 2))
                return response;

           memcpy(
               cparam->value,
               &cmd[1 + NAME_SIZE * 2],
               stype.size
           );
        }

        //return parameter values to CMD_SET_PARAM and CMD_GET_PARAM
        char valueType = (char)cparam->type.type;
        STSize_t valueSize = cparam->type.size;
        rlength = 2 + NAME_SIZE * 2 + valueSize + sizeof(STSize_t);
        response = new char[rlength];

        Buffer rbuff(response, rlength);
        rbuff.setValue<char>(cmdName);
        rbuff.setValueFromPointer<char[NAME_SIZE]>(cname);
        rbuff.setValueFromPointer<char[NAME_SIZE]>(pname);
        rbuff.setValue<char>(valueType);
        rbuff.setValue<STSize_t>(valueSize);
        rbuff.setVoidValue(valueSize, cparam->value);
    }

    return response;
}
