#include "OGConfig.h"

#define TYPE_MASK 2    //0b000000xx
#define NAME_MASK 0x3f //0b00111111

#define GET 0x01
#define SET 0x02
#define POST 0x03

#define LIGHT_CYCLE 0x01
#define HEATER_PID 0x02
#define LIGHT_STATE 0x03
#define HEATER_STATE 0x04
#define SAVE_CONFIG 0x05

#define CMD_FILE "Init.config"

#define BUFF_NULL {}

#include "Buffer.h"

OGConfig::OGConfig(CLight* light, CHeater* heater, uint8_t sdPin) : _light(light), _heater(heater), _sdPin(sdPin)
{
  pinMode(sdPin, OUTPUT);

  if (!SD.begin(sdPin)) {
    while(true);
  }
}

/***********   cmd  *******************************
   first byte follow this bit pattern: TTNNNNNN
   T = type of command, GET or SET
   N = name of command

   others bytes are used to passing arguments

   byte array returned is the reponse, response is empty when responseLength = -1

**************************************************/
byte* OGConfig::onCommand(byte *cmd, int length, int &rlength)
{
  if(length <= 0) return BUFF_NULL;
  
  rlength = -1;
  byte* response;
  
  
  byte type = cmd[0] >> ((byte)8 - (byte)TYPE_MASK); //0bTTNNNNNN to 0b000000TT
  byte name = cmd[0] &  (byte)NAME_MASK;             //0bTTNNNNNN to 0b00NNNNNN

  if(name == SAVE_CONFIG)
  {
    if(type == SET)
    {
      this->writeConfig();
    }
  }
  else if(name == LIGHT_CYCLE)
  {
    if(type == SET && length == 3)
    {
      _light->setCycle((uint8_t) cmd[1], (uint8_t) cmd[2]);
    }
    else if(type == GET)
    {
      uint8_t start = _light->getStartHour();
      uint8_t stop  = _light->getStopHour();

      rlength = 3;
      response = new byte[rlength] {join(POST, LIGHT_CYCLE), (byte) start, (byte) stop };
    }
  }
  else if(name == LIGHT_STATE)
  {
    if(type == SET && length == 2)
    {
      _light->setActive((bool)cmd[1]);
    }
    else if(type == GET)
    {
      rlength = 2;
      response = new byte[rlength]{ join(POST, LIGHT_STATE), (byte)_light->getActive()};
    }
  }
  else if(name == HEATER_STATE)
  {
    if(type == SET && length == 2)
    {
      _heater->setActive((bool)cmd[1]);
    }
    else if(type == GET)
    {
      rlength = 2;
      response = new byte[rlength]{ join(POST, HEATER_STATE), (byte)_heater->getActive()};
    }
  }
  else if(name == HEATER_PID)
  {
    
    if(type == SET && length == (sizeof(double) * 3) + 1)
    {
      Buffer bbuff(cmd, length);
      
      bbuff.setCursor(length);
      
      double Kd = bbuff.getValue<double>();
      double Ki = bbuff.getValue<double>();
      double Kp = bbuff.getValue<double>();
      
      _heater->setPID(Kp, Ki, Kd);
    }
    else if(type == GET)
    {
      rlength = (sizeof(double) * 3) + (sizeof(byte) * 1); 
      Buffer bbuff(response = new byte[rlength], rlength);

      bbuff.setValue<byte>(join(POST, HEATER_PID));
      bbuff.setValue<double>(_heater->getKp());
      bbuff.setValue<double>(_heater->getKi());
      bbuff.setValue<double>(_heater->getKd());
    }
  }

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
      while (true);
    }

    byte buff[] =  { 0x02, join(SET, LIGHT_STATE), (byte)false, 0x02, join(SET, HEATER_STATE), (byte)false };

    config.write(buff, 6);
    config.close();
  }

  File config = SD.open(CMD_FILE);
  if (!config)
  {
    while (true);
  }

  int size = (int) config.size();
  byte buff[size];

  config.read(buff, size);
  config.close();

  for (int i = 0; i < size; i++)
  { 
    byte cmdLength = buff[i];
    
    
    byte cmd[cmdLength];
    for (int j = i + 1; j < cmdLength + i; j++)
      cmd[j] = buff[j];

    int rlength;
    this->onCommand(cmd, cmdLength, rlength);
    
    i += (int) cmdLength;
  }
  
}

void OGConfig::writeConfig()
{

  File config = SD.open(CMD_FILE, FILE_WRITE);
  if (!config)
  {
    while(true);
  }

  size_t dsize = sizeof(double);

  int blength = (sizeof(byte) * 12 ) + (dsize * 3);
  byte buff[blength] =
  {
    0x03, join(SET, LIGHT_CYCLE), (byte) _light->getStartHour(), (byte) _light->getStopHour(),
    0x02, join(SET, LIGHT_STATE), (byte) _light->getActive(),
    0x02, join(SET, HEATER_STATE), (byte) _heater->getActive(),
    (byte)(dsize*3+1), join(SET, HEATER_PID) //3 double...
  };
  
  double Kp = _heater->getKp(), Ki = _heater->getKi(), Kd = _heater->getKd();
  cat(buff, (byte*)&Kp, 12            , dsize);
  cat(buff, (byte*)&Ki, 12 + dsize    , dsize);
  cat(buff, (byte*)&Kd, 12 + dsize * 2, dsize);
  
  config.write(buff, blength);
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
