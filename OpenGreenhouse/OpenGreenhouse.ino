
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

void setup()
{
  Serial.begin(9600);
  
  time = new MTime();
  temp = new MTemp(IN_TEMP);
  
  light = new CLight(time, OUT_LIGHT);
  heater = new CHeater(temp, OUT_HEATER);
  
  config = new OGConfig(light, heater, SD_PIN);
  config->readSavedCommands();
}

void loop()
{
  if(Serial.available() > 0)
  {
    delay(50); // wait data
    int size = Serial.available();
    
    byte data[size];
    Serial.readBytes(data, size);
    
    int rlength;
    byte* response = config->onCommand(data, size, rlength);
    
    if(rlength > 0)
      Serial.write(response, rlength);

    delete[] response;
  }


  static unsigned long lastUpdate = 0;
  long unsigned currTime = millis();
  if((currTime - lastUpdate) > UPDATE_TIME)
  {
    lastUpdate = currTime;
    
    light->update();
    heater->update();
  }
}
