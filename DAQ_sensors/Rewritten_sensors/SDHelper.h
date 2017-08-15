#ifndef SDHELPER_H
#define SDHELPER_H

#include <SD.h>
#include <SPI.h>

const uint8_t chipSelect = 8;
const uint8_t cardDetect = 9;
bool alreadyBegan = false;  // SD.begin() misbehaves if not first call
char fileName[] = "data.csv";

/*
 * Updated initalizeCard method
 * Arduino's SD Library runs into issues if SD.begin isn't the first call
 * Fixes issues we had in the past with card error
 */

void initializeCard(void)
{
  Serial.print(F("Initializing SD card..."));
  // Is there even a card?
  if (!digitalRead(cardDetect)){
    Serial.println(F("No card detected. Waiting for card."));
    while (!digitalRead(cardDetect));
    delay(250); // 'Debounce insertion'
  }

  // Card seems to exist.  begin() returns failure
  // even if it worked if it's not the first call.
  if (!SD.begin(chipSelect) && !alreadyBegan)  // begin uses half-speed...
  {
    Serial.println(F("Initialization failed!"));
    initializeCard(); // Possible infinite retry loop is as valid as anything
  }
  else{
    alreadyBegan = true;
  }
  Serial.println(F("Initialization done."));
  Serial.print(fileName);
  
  if (SD.exists(fileName)){
    SD.remove(fileName);
  }
  // wait for the SD card to finish deleting
  delay(500);

  File sensorData = SD.open(fileName, FILE_WRITE);
  sensorData.close();
  Serial.print("Opening file: ");
  Serial.println(fileName);
}

bool saveData(const String& dataString) {
    if(SD.exists("data.csv")){ // check the card is still there
        // now append new data file
        File sensorData = SD.open("data.csv", FILE_WRITE);
        if (sensorData){
            sensorData.println(dataString);
            sensorData.close(); // close the file
            return true;
        } else {
            return false;
        }
    }
    else{
        return false;
    }
}

#endif // SDHELPER_H
