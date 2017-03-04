#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

long refresh_RotaryPotentiometer(int sensorValue, int degrees)
{
    // Black is Vout
    // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
    float voltage = sensorValue * (5.0 / 1023.0);
    long pos = -(voltage - 2.5)/5*degrees;
    return pos;
}

#endif // POTENTIOMETER_H
