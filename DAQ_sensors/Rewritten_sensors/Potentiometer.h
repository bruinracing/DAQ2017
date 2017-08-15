#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

typedef struct potentiometer {
    short pin;
    short degrees;
} potentiometer;

long refresh_RotaryPotentiometer(potentiometer pot)
{
    // Black is Vout
    // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
    float voltage = analogRead(pot.pin) * (5.0 / 1023.0);
    long pos = -(voltage - 2.5)/5*pot.degrees;
    return pos;
}

#endif // POTENTIOMETER_H
