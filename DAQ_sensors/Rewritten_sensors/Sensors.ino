#include "Potentiometer.h"
#include "HallEffect.h"
#include "IMU.h"
#include "SDHelper.h"

#define DEBUG 0

/* hallEffect leftHall, rightHall; */
potentiometer shockLeftPot, shockRightPot;
LSM9DS1 imu;
#define brakePressureFront A2

//the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD
// library functions will not work.
const int CSpin = 10;

String dataString; // holds the data to be written to the SD card
float sensorReading1 = 0.00; // value read from your first sensor
float sensorReading2 = 0.00; // value read from your second sensor
float gyroX = 0.00; // value read from your third sensor
float gyroY = 0.00;
float gyroZ = 0.00;
float accelX = 0.00;
float accelY = 0.00;
float accelZ = 0.00;

#define PRINT_SPEED 1000 // 250 ms between prints
static unsigned long lastPrint = 0; // Keep track of print time

float shockLeftPos, shockRightPos;
float brakePressureFrontValue;

void setup() {
  Serial.begin(115200);
  pinMode(cardDetect, INPUT);
  initializeCard();
    

    /* rightHall.pinNum=3; */
    /* leftHall.pinNum=4; */
    /* pinMode(rightHall.pinNum,INPUT); */
    /* pinMode(leftHall.pinNum,INPUT); */

    shockLeftPot.pin = A0;
    shockLeftPot.degrees = 360;
    shockRightPot.pin = A1;
    shockRightPot.degrees = 360;

    Serial.println("Beginning IMU initialization.");
    imu.settings.device.commInterface = IMU_MODE_I2C;
    imu.settings.device.mAddress = LSM9DS1_M;
    imu.settings.device.agAddress = LSM9DS1_AG;
    if (!imu.begin())
    {
        Serial.println("Failed to communicate with LSM9DS1.");
        while (1)
            ;
    }
}

// the loop routine runs over and over again forever:
void loop() {
  /*
    shockLeftPos  = refresh_RotaryPotentiometer(shockLeftPot);
    shockRightPos = refresh_RotaryPotentiometer(shockRightPot);
    brakePressureFrontValue = (analogRead(brakePressureFront)*5.0/1023.0-.5)/4*5000;
*/
    /* refresh_WheelSpeed(rightHall); */
    refresh_IMU(imu);
    if ((lastPrint + PRINT_SPEED) < millis())
    {
        gyroX = imu.calcGyro(imu.gx);
        gyroY = imu.calcGyro(imu.gy);
        gyroZ = imu.calcGyro(imu.gz);
        //accel readings
        accelX = imu.calcAccel(imu.ax);
        accelY = imu.calcAccel(imu.ay);
        accelZ = imu.calcAccel(imu.az);
        lastPrint = millis(); // Update lastPrint time
    }
    // build the data string
    
    //dataString = String(sensorReading1) + ",";
    //dataString += String(sensorReading2) + ",";
    dataString = String(gyroX) + ",";
    dataString += String(gyroY) + ",";
    dataString += String(gyroZ) + ",";
    dataString += String(accelX) + ",";
    dataString += String(accelY) + ",";    
    dataString += String(accelZ) + ","; // for IMU
    
    dataString += String(shockLeftPos)+ ",";
    dataString += String(shockRightPos)  + ",";
    dataString += String(brakePressureFrontValue) + ",";
    

    // Debug print statements
    #if DEBUG
    Serial.print( "Gyro X: " );
    Serial.println(gyroX);
    Serial.print( "Gyro Y: " );
    Serial.println(gyroY);
    Serial.print( "Gyro Z: " );
    Serial.println(gyroZ);
    
    Serial.print( "Accel X: " );
    Serial.println(accelX);
    Serial.print( "Accel Y: " );
    Serial.println(accelY);
    Serial.print( "Accel Z: " );
    Serial.println(accelZ);
    #endif
    
    //Serial.println ( brakePressureFrontValue );
    // convert to CSV
    saveData(dataString); // save to SD card
}


