#include "OGConfig.h"
#include "Buffer.h"

OGConfig::OGConfig(IConfigurable** configurables, int clength, uint8_t sdpin)
            : configurables(configurables), configLength(clength)
{
    saver = new ConfigSaver(this, sdpin);
    
    paramsLength = 0;
    for(int i = 0; i < clength; i++)
    {
        int length;
        SParameter* params = configurables[i]->getParams(length);
        
        for(int j = 0; j < length; j++)
            if(params[j].saveable) saveablesLength++;
        
        paramsLength += length;
    }
}

OGConfig::~OGConfig()
{
    delete saver;
}

char* OGConfig::onCommand(char* cmd, int clength, int& rlength)
{
    rlength = 0;
    char* response = nullptr;
    
    if(clength == 0)
        return response;
    
    char cmdName = cmd[0];

    if(cmdName == CMD_GET_ALL || cmdName == CMD_GET_SAVABLES) // <cmd>, [<id_config>, <id_param>, <type>, <size>, <value>]
    {
        int sizep = (cmdName == CMD_GET_ALL) ? this->paramsLength : this->saveablesLength;
        
        rlength = (1 + NAME_SIZE * 2 + 1 + sizeof(STSize_t)) + sizep;
        response = new char[rlength];

        Buffer buff(response, rlength);
        buff.setValue<char>(cmdName);
        
        for(int i = 0; i < this->configLength; i++) //search configurable and take her params
        {
            IConfigurable* conf = this->configurables[i];

            int plength;
            SParameter* params = conf->getParams(plength);
            
            for(int j = 0; j < plength; j++)
            {
                SParameter param = params[j];
                if(cmdName == CMD_GET_SAVABLES && !param.saveable)
                    continue;
                
                buff.setValue<OGName>(conf->getConfigName());
                buff.setValue<OGName>(param.name);
                buff.setValue<char>(static_cast<char>(param.type.type));
                buff.setValue<STSize_t>(param.type.size);
                buff.setVoidValue(param.type.size, param.value);
            }
        }
    }
    else if(cmdName == CMD_GET_PARAM || cmdName == CMD_SET_PARAM || cmdName == CMD_BLIND_SET_PARAM)
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
        if(cmdName != CMD_BLIND_SET_PARAM)
        {
            char valueType = static_cast<char>(cparam->type.type);
            STSize_t valueSize = cparam->type.size;
            rlength = 2 + NAME_SIZE * 2 + valueSize + sizeof(STSize_t);
            response = new char[rlength];
            
            Buffer rbuff(response, rlength); //cmdName configName paramName valueType valueSize value
            rbuff.setValue<char>(cmdName);
            rbuff.setValueFromPointer<char[NAME_SIZE]>(cname);
            rbuff.setValueFromPointer<char[NAME_SIZE]>(pname);
            rbuff.setValue<char>(valueType);
            rbuff.setValue<STSize_t>(valueSize);
            rbuff.setVoidValue(valueSize, cparam->value);
        }
    }
    else if(cmdName == CMD_SAVE)
    {
        this->saver->saveConfig();
    }
    else if(cmdName == CMD_READ)
    {
        this->saver->readConfig();
    }

    return response;
}

void OGConfig::onCommand(char* cmd, int clength)
{
    int rlength;
    char* response = this->onCommand(cmd, clength, rlength);
    delete[] response;
}
