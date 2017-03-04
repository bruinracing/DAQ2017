#ifndef SDHELPER_H
#define SDHELPER_H

#include <SD.h>
#include <SPI.h>

bool initSD(int pin) {
    pinMode(pin, OUTPUT);
    digitalWrite(10, HIGH);
    if (!SD.begin(pin)) {
        return false;
    }
    File sensorData = SD.open("data.csv", FILE_WRITE);
    sensorData.close();
    return true;
}

bool saveData(String dataString) {
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
