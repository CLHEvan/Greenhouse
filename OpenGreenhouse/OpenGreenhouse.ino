#include "OGConfig.h"
#include "STime.h"
#include "STemp.h"
#include "CHeater.h"
#include "CLight.h"

#define SERIAL_BUFF_SIZE 64

#define TEMP_SENSOR_PIN  5
#define HEATER_PIN 6
#define SD_PIN 10

STime time;
STemp temp(TEMP_SENSOR_PIN);
CHeater heater(&temp, HEATER_PIN, 27.0, 4.0);
CLight light(&time, 8, 22);

IConfigurable* configurables[3] =
{
     (IConfigurable*) &time,
     (IConfigurable*) &heater,
     (IConfigurable*) &light
};

OGConfig config(configurables, 3, SD_PIN);

void setup()
{
    Serial.begin(115200);

    time.begin();

    char cmd[1] = { CMD_READ };
    config.onCommand(cmd, 1);
}

void loop()
{

    if(Serial.available() > 0)
    {
        delay(500);//wait data
        
        char buff[SERIAL_BUFF_SIZE];
        int readLength = (int)Serial.readBytes(buff, SERIAL_BUFF_SIZE); //return size_t

        int rlength;
        char* response = config.onCommand(buff, readLength, rlength);

        Serial.write(response, rlength);
        
        delete[] response;//TODO find an other way to del response, because is OGConfig who create the response so is to OGConfig to delete the response
    }
    
    //no require specific time to update, but i put a delay to save mcu resources
    delay(1000);
    
    time.update();
    heater.update();
    light.update();
}
