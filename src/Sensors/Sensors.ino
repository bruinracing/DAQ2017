#include "Potentiometer.h"
#include "HallEffect.h"
#include "IMU.h"
#include "SDHelper.h"

/* hallEffect leftHall, rightHall; */
potentiometer shockLeftPot, shockRightPot;
LSM9DS1 imu;
#define brakePressureFront A2

//the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD
// library functions will not work.
const int CSpin = 10;
String dataString = ""; // holds the data to be written to the SD card
float sensorReading1 = 0.00; // value read from your first sensor
float sensorReading2 = 0.00; // value read from your second sensor
float sensorReading3 = 0.00; // value read from your third sensor
float sensorReading4 = 0.00;
float sensorReading5 = 0.00;
float sensorReading6 = 0.00;
float sensorReading7 = 0.00;
float sensorReading8 = 0.00;

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
        sensorReading3 = imu.calcGyro(imu.gx);
        sensorReading4 = imu.calcGyro(imu.gy);
        sensorReading5 = imu.calcGyro(imu.gz);
        //accel readings
        sensorReading6 = imu.calcAccel(imu.ax);
        sensorReading7 = imu.calcAccel(imu.ay);
        sensorReading8 = imu.calcAccel(imu.az);
        lastPrint = millis(); // Update lastPrint time
    }
    // build the data string
    dataString = String(sensorReading1) + "," +
                 String(sensorReading2) + "," +
                 String(sensorReading3) + "," +
                 String(sensorReading4) + "," +
                 String(sensorReading5) + "," +
                 String(sensorReading6) + "," +
                 String(sensorReading7) + "," +
                 String(sensorReading8) + "," + // for IMU
                 String(shockLeftPos)   + "," +
                 String(shockRightPos)  + "," +
                 String(brakePressureFrontValue) + ",";
    // convert to CSV
    saveData(dataString); // save to SD card
}
