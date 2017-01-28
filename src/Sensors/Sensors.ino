
#define steeringPot A0
#define shockPot A1
const double wheelCircumference = 10/12.0/5280*PI; // in miles
const int pulsesPerRev = 4;
const int movingAverageNum = 4;
struct hallEffect{
  bool curState;
  bool pastState;
  double timetemp1;
  double wheelSpeed;
  int counter;
  int pinNum;
  double wheelSpeeds[pulsesPerRev];
double * head = wheelSpeeds;
}leftHall,rightHall;
/*struct Node{
  Node* nextNode;
  double wheelSpeed;
} wheelNode;
*/
//Node* head;
long steeringPos;
long shockPos;
void refresh_WheelSpeed(hallEffect & sensor);
long refresh_RotaryPotentiometer(int sensorValue, int degrees);

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
long refresh_RotaryPotentiometer(int sensorValue, int degrees)
{
  // Black is Vout
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = sensorValue * (5.0 / 1023.0);
  long pos = -(voltage - 2.5)/5*degrees;
  return pos;
}
void refresh_WheelSpeed(hallEffect & sensor) // this is current set to one click per rotation
{

  sensor.curState = digitalRead(sensor.pinNum);

  if (sensor.curState&&!sensor.pastState)
    {
      double timetemp2= sensor.timetemp1; //in hours
      sensor.timetemp1= micros()/3600.0/1E6; //in hours
      double timetemp3= sensor.timetemp1-timetemp2; // in hours
      sensor.wheelSpeed-=*sensor.head;
  //Serial.println(timetemp3*3600, 6);
  *sensor.head= 1.0/timetemp3/pulsesPerRev/movingAverageNum/60; // to calculate RPM, then divide by movingAverageNum to get weighted value
    // *sensor.head= wheelCircumference/timetemp3/pulsesPerRev/movingAverageNum; // get new instantaneous speed in mi/hr, then divide by moving AverageNum to get weighted value 
      sensor.wheelSpeed+=*sensor.head;
      sensor.head= sensor.wheelSpeeds+((++sensor.counter)%(movingAverageNum));
     //  Serial.println(sensor.wheelSpeed);
    }
    sensor.pastState = sensor.curState;
  
}

