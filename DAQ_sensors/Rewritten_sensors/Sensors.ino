#include "Potentiometer.h"
#include "HallEffect.h"
#include "IMU.h"
#include "SDHelper.h"
#include <Time.h>

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

#define PRINT_SPEED 250 // 250 ms between prints
static unsigned long lastPrint = 0; // Keep track of print time

float shockLeftPos, shockRightPos;
float brakePressureFrontValue;

void setup() {
    Serial.begin(115200);

    if (!initSD(CSpin)) {
        Serial.println("Card error");
        while(1);
    }

    /* rightHall.pinNum=3; */
    /* leftHall.pinNum=4; */
    /* pinMode(rightHall.pinNum,INPUT); */
    /* pinMode(leftHall.pinNum,INPUT); */

    shockLeftPot.pin = A0;
    shockLeftPot.degrees = 360;
    shockRightPot.pin = A1;
    shockRightPot.degrees = 360;

    Serial.println("Beginning IMU initialization.");
    // Before initializing the IMU, there are a few settings
    // we may need to adjust. Use the settings struct to set
    // the device's communication mode and addresses:
    imu.settings.device.commInterface = IMU_MODE_I2C;
    imu.settings.device.mAddress = LSM9DS1_M;
    imu.settings.device.agAddress = LSM9DS1_AG;
    // The above lines will only take effect AFTER calling
    // imu.begin(), which verifies communication with the IMU
    // and turns it on.
    if (!imu.begin())
    {
        Serial.println("Failed to communicate with LSM9DS1.");
        Serial.println("Double-check wiring.");
        Serial.println("Default settings in this sketch will " \
                "work for an out of the box LSM9DS1 " \
                "Breakout, but may need to be modified " \
                "if the board jumpers are.");
        while (1)
            ;
    }
    //saveData(now());
    
    String titles = "Gyro X, Gyro Y, Gyro Z, Accel X, Accel Y, Accel Z, Left Shock, Right Shock, Brake Pressure,";
    saveData(titles);
    titles = "";
}

// the loop routine runs over and over again forever:
void loop() {
    shockLeftPos  = refresh_RotaryPotentiometer(shockLeftPot);
    shockRightPos = refresh_RotaryPotentiometer(shockRightPot);
    brakePressureFrontValue = (analogRead(brakePressureFront)*5.0/1023.0-.5)/4*5000;

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
    
    //Serial.println ( brakePressureFrontValue );
    // convert to CSV
    saveData(dataString); // save to SD card
}
