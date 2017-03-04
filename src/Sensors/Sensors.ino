#include "Potentiometer.h"
#include "HallEffect.h"

#define steeringPot A0
#define shockPot A1

/*struct Node{
  Node* nextNode;
  double wheelSpeed;
  } wheelNode;
 */
//Node* head;

struct hallEffect leftHall, rightHall;
long steeringPos, shockPos;

void setup() {
    Serial.begin(9600);
    rightHall.pinNum=3;
    leftHall.pinNum=4;
    pinMode(rightHall.pinNum,INPUT);
    pinMode(leftHall.pinNum,INPUT);
}


// the loop routine runs over and over again forever:
void loop() {
    steeringPos=refresh_RotaryPotentiometer(analogRead(steeringPot),90);
    shockPos= refresh_RotaryPotentiometer(analogRead(shockPot),360);
    Serial.println(steeringPos);
    refresh_WheelSpeed(rightHall);
}
