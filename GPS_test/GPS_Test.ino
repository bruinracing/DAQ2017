#include "GPS_helper.h"

SerialGPS gps(0,1); 

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  gps.getCoords();
  
}

