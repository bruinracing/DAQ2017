#include "Potentiometer.h"
#include "HallEffect.h"

hallEffect leftHall, rightHall;
potentiometer steeringPot, shockPot;
long steeringPos, shockPos;

void setup() {
    Serial.begin(9600);

    rightHall.pinNum=3;
    leftHall.pinNum=4;
    pinMode(rightHall.pinNum,INPUT);
    pinMode(leftHall.pinNum,INPUT);

    steeringPot.pin = A0;
    steeringPot.degrees = 90;
    shockPot.pin = A1;
    shockPot.degrees = 360;
}

// the loop routine runs over and over again forever:
void loop() {
    steeringPos=refresh_RotaryPotentiometer(steeringPot);
    shockPos= refresh_RotaryPotentiometer(shockPot);
    Serial.println(steeringPos);

    refresh_WheelSpeed(rightHall);
}
