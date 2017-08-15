#include <Wire.h>
#include <Adafruit_MCP4725.h>

// 100 -> 90 : 23 -> 17
// 90 -> 80 : 17 - 13

/*  Values: end of 4/7/2017
 *   LOWER: 100, UPPER: 240
 *   LOWER: 85, UPPER: 240 - idle
 *  Test cases:
 *    Lower|Upper
 *    100|245
 *    100|250
 *    100|251
 *    100|252 
 *    100|253
 *    100|254
 */

#define INPUT_PIN A1
#define n_SAMPLES 10.0

//bounds for the output - changing lower changes the output degree of 0 
//                      - changing upper changes the output degree of MAX_TPS  
#define LOWER 1400  // on a scale of  0-4095
#define UPPER 3900
#define UPPER_BOUND 10

#define MAX_TPS_IN 800   // input scale is 0 - 1023
#define MIN_TPS_IN 100

#define POWERUP 0.333   // the scaling factor that we use to generate table
#define POWERDOWN 3    
#define EXTRA 100       // on a scale of 0-1023
                        // TPS cutoff for d(θ)/dt for 20 ticks
#define dTPSTableSize 20
Adafruit_MCP4725 dac;
static unsigned short table_up[(MAX_TPS_IN - MIN_TPS_IN+1)];
static unsigned short table_dTPS[dTPSTableSize];
//static unsigned short table_down[(MAX_TPS_IN - MIN_TPS_IN + 1)/2]; 

// variable for calculating moving average
short accumulated[15];
int head = 0; 
int movingAv = 0;
int arrSize = 15;
long timeStart;

// the range for TPS
int TPS_DIFF; 

#define PRINT 0

int pTPS;  //previous TPS

void setup() {
  TPS_DIFF =MAX_TPS_IN - MIN_TPS_IN + 1;
  pTPS = 0;
#if PRINT
  Serial.begin(115200);
  Serial.println("Help");
#endif
  Serial.begin(115200);
  pinMode(INPUT_PIN, INPUT);
  dac.begin(0x62);

  //creates a lookup table of TPS values
  for (int i = 0; i < TPS_DIFF; i++) {
    float x = (((float)i)/( TPS_DIFF));
    x = pow(x,POWERUP);
    x = x * (UPPER-LOWER);
    unsigned short res1 = ((unsigned short)x) + LOWER;
    //unsigned short res2 = ((unsigned short)y) + LOWER;
    if (res1 > UPPER) res1 = UPPER;
    if (res1 < LOWER) res1 = LOWER; 
    table_up[i] = res1;
#if PRINT
    Serial.print(i+MIN_TPS_IN);
    Serial.print(" ");
    Serial.println(table_up[i]);
#endif
  
  }
  for (int i =0; i <dTPSTableSize/2; i++) {
     float x = ((float) i) / dTPSTableSize/2;
    x = pow(x,.5);
    x = x* (UPPER-LOWER);
    table_dTPS[i+dTPSTableSize/2] = (short) x;
    table_dTPS[dTPSTableSize/2-i] = (short) -x;
  }
  //Serial.println("NO");
}

void loop() {
  long v_in = analogRead(INPUT_PIN); //read current TPS
  int i_in = v_in - MIN_TPS_IN;     // i_in is a value used to calculate the value in the lookup table
  int idx;
//  if (movingAv - i_in > EXTRA) i_in = i_in + 150;   
//  else if (i_in - movingAv > EXTRA) i_in = i_in - 150;


  double TPS = 0;
  for(int i = 0; i < n_SAMPLES; i++){
    TPS += analogRead( INPUT_PIN ) / n_SAMPLES;
  }
  double rawdTPS = (TPS - pTPS)/n_SAMPLES;
  pTPS = TPS;
  

  Serial.print("dTPS: ");
  Serial.print(rawdTPS);
  Serial.print(" TPS: ");
  Serial.println( TPS );
  int dTPS = rawdTPS;
  int dTPSIndex;
  if (dTPS < -dTPSTableSize/2)
  dTPSIndex = 0;
  else if (dTPS > dTPSTableSize/2)
  dTPSIndex = dTPSTableSize-1;
  else
   dTPSIndex = dTPS+dTPSTableSize/2;
  short TPSAdjust = table_dTPS[dTPSIndex];
 i_in = i_in +TPSAdjust;
  if (i_in < 0) idx = 0;
  else if (i_in > TPS_DIFF) idx = TPS_DIFF;
  else idx = i_in;

  int v_out = 0;
  /*
  movingAv = movingAv + ( (v_in - accumulated[head]) / arrSize );
  accumulated[head] = v_in;

  if( accumulatedTime > 1000 ){
    
  }
  
  head++;
  if(head == arrSize){
    head = 0;
  }
  Serial.println( movingAv );*/
  v_out = table_up[idx];
  
  dac.setVoltage(v_out, false);
  Serial.println (v_out);
#if PRINT
  Serial.print(v_in);
  Serial.print(" ");
  Serial.println(v_out);
  delay(100);
#endif
}


