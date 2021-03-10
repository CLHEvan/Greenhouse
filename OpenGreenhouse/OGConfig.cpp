#include "OGConfig.h"

#define TYPE_MASK 2    //0b000000xx
#define NAME_MASK 0x3f //0b00111111

#define GET 0x01
#define SET 0x02
#define POST 0x03

#define LIGHT_CYCLE 0x01   //get or set light cycle
#define HEATER_PID 0x02    //get or set light cycle
#define LIGHT_STATE 0x03
#define HEATER_STATE 0x04
#define SAVE_CONFIG 0x05

#define CMD_FILE "Init.config"

#define BUFF_NULL {}

OGConfig::OGConfig(CLight* light, CHeater* heater, uint8_t sdPin) : _light(light), _heater(heater), _sdPin(sdPin)
{
  pinMode(sdPin, OUTPUT);

  if (!SD.begin(sdPin)) {
    //log
    for (;;);
  }
}

void OGConfig::onCommand(byte *cmd, int length)
{
  int len;
  this->onCommand(cmd, length, len);
}

/***********   cmd  *******************************
   first byte follow this bit pattern: TTNNNNNN
   T = type of command, GET or SET
   N = name of command

   others bytes are used to passing arguments

   byte array returned is the reponse, response is empty when responseLength = -1

  \**************************************************/
byte* OGConfig::onCommand(byte *cmd, int length, int &responseLength) // = -1
{
  if (length <= 0) return BUFF_NULL;
  responseLength = -1;
  byte* response;
  
  byte type = cmd[0] >> ((byte)8 - (byte)TYPE_MASK); //0bTTNNNNNN to 0b000000TT
  byte name = cmd[0] &  (byte)NAME_MASK;             //0bTTNNNNNN to 0b00NNNNNN

  switch (name)
  {
    case LIGHT_CYCLE:
      break;
    case LIGHT_STATE:
      if (type == SET && length == 2)
        _light->setActive(true);
      else if (type == GET)
      {
        byte resp[] = { join(POST, LIGHT_STATE), (byte)_light->getActive()};
        response = resp;
        responseLength = 2;
      }
      break;

    case HEATER_PID:
      if (type == SET && length == 25)
      {
        _heater->setKp(getDouble(cmd, 1));
        _heater->setKi(getDouble(cmd, 9));
        _heater->setKd(getDouble(cmd, 17));
      }
      else if (type == GET) //cmd double double double   1 + 8*3
      {
        double Kp = _heater->getKp(), Ki = _heater->getKi(), Kd = _heater->getKd();
        byte resp[25];
        resp[0] = join(POST, HEATER_PID);
        cat(resp, (byte*)&Kp, 1, 8);   //0xff 00 00 00 00 00 00 00 00 11 11 11 11 11 11 11 11 00 00 00 00 00 00 00 00
        cat(resp, (byte*)&Ki, 9, 8);   //  0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24
        cat(resp, (byte*)&Kd, 17, 8);
        responseLength = 25;
      }
      break;

    case HEATER_STATE:
      if (type == SET && length == 2)
        _heater->setActive(true);
      else if (type == GET)
      {
        byte resp[] = {join(POST, LIGHT_STATE), (byte)_light->getActive()};
        response = resp;
        responseLength = 2;
      }
      break;

    case SAVE_CONFIG:
      writeConfig();
      break;
    default:
      break;
  }

  if (responseLength == -1)
    return BUFF_NULL;

  return response;
}

/*
   eache commande begin with one byte to indicate her length, this byte will be removed in the read
*/
void OGConfig::readSavedCommands()
{
  if (!SD.exists(CMD_FILE)) //create default file with command light and heater state define to false
  {
    File config = SD.open(CMD_FILE, FILE_WRITE);
    if (!config)
    {
      //serial . . .
      while (true);
    }

    byte buff[] =  { 0x02, 0x83, 0x00, 0x02, 0x84, 0x00 };

    config.write(buff, 6);
    config.close();
  }

  File config = SD.open(CMD_FILE);
  if (!config)
  {
    //serial . . .
    while (true);
  }

  int size = (int) config.size(); //.size() return long
  byte buff[size];

  config.read(buff, size);
  config.close();

  for (int i = 0; i < size; i++)
  { 
    byte cmdLength = buff[i];
    
    
    byte cmd[cmdLength];
    for (int j = i + 1; j < cmdLength + i; j++)
      cmd[j] = buff[j];
    
    this->onCommand(cmd, cmdLength);
    
    i += (int) cmdLength;
  }
  
}

void OGConfig::writeConfig()
{

  File config = SD.open(CMD_FILE, FILE_WRITE);
  if (!config)
  {
    return; //block the program can be dangerous for the greenhouse
    //serial . . .
    //while(true);
  }

  byte buff[36] =
  {
    0x03, join(SET, LIGHT_CYCLE), (byte) _light->getStartHour(), (byte) _light->getStopHour(),
    0x02, join(SET, LIGHT_STATE), (byte) _light->getActive(),
    0x02, join(SET, HEATER_STATE), (byte) _heater->getActive(),
    0x19, join(SET, HEATER_PID) //3 double...
  };
  
  double Kp = _heater->getKp(), Ki = _heater->getKi(), Kd = _heater->getKd();
  cat(buff, (byte*)&Kp, 12, 8);
  cat(buff, (byte*)&Ki, 20, 8);
  cat(buff, (byte*)&Kd, 28, 8);
  
  config.write(buff, 36);
  config.close();
}

byte OGConfig::join(byte type, byte name)
{
  type = type << ((byte)8 - (byte)TYPE_MASK);
  return name + type;
}

void OGConfig::cat(byte* buff, byte* buff2, int start, int length)
{
  for (int i = 0; i < length; i++)
    buff[start + i] = buff2[i];
}

double OGConfig::getDouble(byte* buff, int start)
{
  byte data[8];
  for(int i = 0; i < 8; i++)
    data[i] = buff[i + start];
  
  double value;
  memcpy(&value, data, 8);
  
  return value;
}
