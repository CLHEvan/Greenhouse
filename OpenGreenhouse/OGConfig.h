#ifndef OGCONFIG_H
#define OGCONFIG_H

#include "Arduino.h"
#include <SD.h>
#include <SD.h>

#include "CLight.h"
#include "CHeater.h"

/****************************
 * OGConfig class
 * 
 * This class provide a command system to be simple use 
 * with a data flux(serial, wifi, isp...) to control the arduino.
 * 
 * The class can also save some commands, it's useful to 
 * keep classes configuration when the arduino is off
 * 
 ***************************/
class OGConfig
{
public:
  OGConfig(CLight* light, CHeater* heater, uint8_t sdPin);
  
  void onCommand(byte *cmd, int length);
  byte* onCommand(byte *cmd, int length, int &responseLength);
  
  void readSavedCommands();
  void writeConfig();
  
private:
  CLight* _light;
  CHeater* _heater;
  
  uint8_t _sdPin;

  byte join(byte type, byte name);
  void cat(byte* buff, byte* buff2, int start, int length);
  double getDouble(byte* buff, int start);
};

#endif
