#ifndef HALLEFFECT_H
#define HALLEFFECT_H

const double wheelCircumference = 10/12.0/5280*PI; // in miles
const int    pulsesPerRev       = 4;
const int    movingAverageNum   = 4;

typedef struct hallEffect {
    bool   curState;
    bool   pastState;
    double timetemp1;
    double wheelSpeed;
    int    counter;
    int    pinNum;
    double wheelSpeeds[pulsesPerRev];
    double *head = wheelSpeeds;
} hallEffect;

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

#endif // HALLEFFECT_H
