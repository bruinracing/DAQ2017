#ifndef IMU_H
#define IMU_H

#include <Wire.h>
#include <SparkFunLSM9DS1.h>

// SDO_XM and SDO_G are both pulled high, so our addresses are:
#define LSM9DS1_M   0x1E // Would be 0x1C if SDO_M is LOW
#define LSM9DS1_AG  0x6B // Would be 0x6A if SDO_AG is LOW

// Earth's magnetic field varies by location. Add or subtract
// a declination to get a more accurate heading. Calculate
// your's here:
// http://www.ngdc.noaa.gov/geomag-web/#declination
#define DECLINATION 12.1 // Declination (degrees) in LA.

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

void refresh_IMU(LSM9DS1 &imu) {
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
}

#endif // IMU_H
