#include "OGConfig.h"

#include <SPI.h>
#include <SD.h>
#include "Buffer.h"

#define FILE_NAME "Config.dat"

ConfigSaver::ConfigSaver(OGConfig* config, uint8_t sdpin) : config(config)
{
    pinMode(sdpin, OUTPUT);
    if(!SD.begin(sdpin))
    {
        Serial.println("ERROR: SD begin error");
        abort();
    }
}

bool ConfigSaver::saveConfig()
{
    File file = SD.open(FILE_NAME, FILE_WRITE);

    if(!file)
    {
        Serial.println("ERROR: Can't open file");
        abort();
    }

    int length;
    char* buff = config->onCommand(CMD_GET_SAVABLES, 1, length);

    if(length == 0)
    {
        Serial.println("ERROR: CMD_GET_SAVABLES return 0 length");
        return false;
    }
    
    file.write(buff, length);
    file.close();

    delete[] buff;
    return true;
}

bool ConfigSaver::readConfig()
{
    if(!SD.exists(FILE_NAME))
        return;
    
    File file = SD.open(FILE_NAME, FILE_READ);
    
    if(!file)
    {
        Serial.println("ERROR: Can't open file");
        abort();
    }
    
    int length = file.available();
    char* sbuff = new char[length];

    file.read(sbuff, length);
    file.close();

    Buffer buff(sbuff, length);
    buff.setCursor(1); // first byte is command name
    
    const int names = NAME_SIZE * 2;
    
    while((int)buff.getCursor() < length - 1)
    {
        int cmdStart = buff.getCursor();
        buff.setCursor(names + 1);
        
        STSize_t vlength = buff.getValue<STSize_t>();

        int clength = names + vlength + 1;
        char* cmd = new char[clength];

        cmd[0] = CMD_BLIND_SET_PARAM;
        memcpy(&cmd[1], &sbuff[cmdStart], names);
        memcpy(&cmd[names + 1], &sbuff[cmdStart + names + 1 + sizeof(STSize_t)], vlength);

        config->onCommand(cmd, clength);
        
        delete[] cmd;
    }

    delete[] sbuff;
    return true;
}
