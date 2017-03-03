//IMU + AnalogRead logging to SD over SPI.
#include <SD.h>
#include <SPI.h>
#include <Wire.h>
#include <SparkFunLSM9DS1.h>

// Use the LSM9DS1 class to create an object. [imu] can be
// named anything, we'll refer to that throught the sketch.
LSM9DS1 imu;

// SDO_XM and SDO_G are both pulled high, so our addresses are:
#define LSM9DS1_M   0x1E // Would be 0x1C if SDO_M is LOW
#define LSM9DS1_AG  0x6B // Would be 0x6A if SDO_AG is LOW

////////////////////////////
// Sketch Output Settings //
////////////////////////////
#define PRINT_CALCULATED
//#define PRINT_RAW
#define PRINT_SPEED 250 // 250 ms between prints
static unsigned long lastPrint = 0; // Keep track of print time

// Earth's magnetic field varies by location. Add or subtract 
// a declination to get a more accurate heading. Calculate 
// your's here:
// http://www.ngdc.noaa.gov/geomag-web/#declination
#define DECLINATION 12.1 // Declination (degrees) in LA.

//the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD
// library functions will not work.
const int CSpin = 10;
String dataString =""; // holds the data to be written to the SD card
float sensorReading1 = 0.00; // value read from your first sensor
float sensorReading2 = 0.00; // value read from your second sensor
float sensorReading3 = 0.00; // value read from your third sensor
float sensorReading4 = 0.00;
float sensorReading5 = 0.00;
float sensorReading6 = 0.00;
float sensorReading7 = 0.00;
float sensorReading8 = 0.00;
File sensorData;

void setup()
{
  // Open serial communications
  Serial.begin(115200);
  Serial.print("Initializing SD card...\n");
  pinMode(CSpin, OUTPUT);
  digitalWrite(10, HIGH);
  //
  // see if the card is present and can be initialized:
  if (!SD.begin(CSpin)) {
    Serial.println("Card failed");
    // don't do anything more:
    Serial.println("WTF\n");
    return;
  }
  sensorData = SD.open("data.csv", FILE_WRITE);
  sensorData.close();
  Serial.println("card initialized.");
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

// Calculate pitch, roll, and heading.
// Pitch/roll calculations take from this app note:
// http://cache.freescale.com/files/sensors/doc/app_note/AN3461.pdf?fpsp=1
// Heading calculations taken from this app note:
// http://www51.honeywell.com/aero/common/documents/myaerospacecatalog-documents/Defense_Brochures-documents/Magnetic__Literature_Application_notes-documents/AN203_Compass_Heading_Using_Magnetometers.pdf
void printAttitude(float ax, float ay, float az, float mx, float my, float mz)
{
  float roll = atan2(ay, az);
  float pitch = atan2(-ax, sqrt(ay * ay + az * az));
  
  float heading;
  if (my == 0)
    heading = (mx < 0) ? PI : 0;
  else
    heading = atan2(mx, my);
    
  heading -= DECLINATION * PI / 180;
  
  if (heading > PI) heading -= (2 * PI);
  else if (heading < -PI) heading += (2 * PI);
  else if (heading < 0) heading += 2 * PI;
  
  // Convert everything from radians to degrees:
  heading *= 180.0 / PI;
  pitch *= 180.0 / PI;
  roll  *= 180.0 / PI;
  
  Serial.print("Pitch, Roll: ");
  Serial.print(pitch, 2);
  Serial.print(", ");
  Serial.println(roll, 2);
  Serial.print("Heading: "); Serial.println(heading, 2);
}

void loop(){
  //These two reads are just raw voltage as a float from A0 and A1.
  int A0 = analogRead(0);
  sensorReading1 = A0;
  sensorReading1 = (sensorReading1 * 5.0) / 1023.0;
  
  int A1 = analogRead(1);
  sensorReading2 = A1;
  sensorReading2 = (sensorReading2 * 5.0) / 1023.0;

    //Begin IMU data collection
    // Update the sensor values whenever new data is available
  if ( imu.gyroAvailable() )
  {
    // To read from the gyroscope,  first call the
    // readGyro() function. When it exits, it'll update the
    // gx, gy, and gz variables with the most current data.
    imu.readGyro();
  }
  if ( imu.accelAvailable() )
  {
    // To read from the accelerometer, first call the
    // readAccel() function. When it exits, it'll update the
    // ax, ay, and az variables with the most current data.
    imu.readAccel();
  }
  if ( imu.magAvailable() )
  {
    // To read from the magnetometer, first call the
    // readMag() function. When it exits, it'll update the
    // mx, my, and mz variables with the most current data.
    imu.readMag();
  }
  
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
  dataString = String(sensorReading1) + "," + String(sensorReading2) + "," + String(sensorReading3) + "," + String(sensorReading4) + "," + String(sensorReading5) + "," + String(sensorReading6) + "," + String(sensorReading7) + "," + String(sensorReading8) + ","; // convert to CSV
  saveData(); // save to SD card
  //delay(60000); // delay before next write to SD Card, adjust as required
}

void saveData(){
  if(SD.exists("data.csv")){ // check the card is still there
    // now append new data file
    sensorData = SD.open("data.csv", FILE_WRITE);
    if (sensorData){
      sensorData.println(dataString);
      sensorData.close(); // close the file
    }
  }
  else{ 
    Serial.println("Error writing to file !");
  }
}
