
#include "ITime.h"
#include "ITemp.h"
#include "MTime.h"
#include "MTemp.h"
#include "CLight.h"
#include "CHeater.h"
#include "OGConfig.h"

#define UPDATE_TIME 1000u

#define OUT_LIGHT 3
#define OUT_HEATER 4

#define IN_TEMP A1

#define SD_PIN 10


ITime* time;
ITemp* temp;

CLight* light;
CHeater* heater;

OGConfig* config;

uint32_t lastUpdate = 0u;

void setup()
{
  Serial.begin(115200);
  
  time = new MTime();
  temp = new MTemp(IN_TEMP);
  
  light = new CLight(time, OUT_LIGHT);
  heater = new CHeater(temp, OUT_HEATER);
  
  config = new OGConfig(light, heater, SD_PIN);
  config->readSavedCommands();
}

void loop()
{
  int size;
  if(size = Serial.available() > 0)
  {
    byte data[size];
    Serial.readBytes(data, size);
    
    int respLength;
    byte* response = config->onCommand(data, size, respLength);
    
    if(respLength != -1)
      Serial.write(response, respLength);
  }
  
  long currTime = millis();
  
  if( ( currTime - lastUpdate ) > UPDATE_TIME)
  {
    lastUpdate = currTime;
    
    //updates
    light->update();
    heater->update();
    //
  }
}
