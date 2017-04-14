#include <Wire.h>
#include <Adafruit_MCP4725.h>

#define INPUT_PIN A1
//#define OUTPUT_PIN 6

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

//bounds for the output - changing lower changes the output degree of 0 
//                      - changing upper changes the output degree of MAX_TPS  
#define LOWER 1400
#define UPPER 3900

#define MAX_TPS_IN 800
#define MIN_TPS_IN 100
#define POWERUP 0.333
#define POWERDOWN 3
#define EXTRA 100

//0.52-4.7 -> 3-235
//0.55-3.9 -> 3-195

Adafruit_MCP4725 dac;
static unsigned short table_up[(MAX_TPS_IN - MIN_TPS_IN+1)/2];
static unsigned short table_down[(MAX_TPS_IN - MIN_TPS_IN + 1)/2]; 

int accumulated[20];
int TPS_DIFF =MAX_TPS_IN - MIN_TPS_IN + 1;

#define PRINT 0

int pTPS;  //previous TPS

void setup() {
#if PRINT
  Serial.begin(115200);
#endif
  pinMode(INPUT_PIN, INPUT);
  dac.begin(0x62);
  //pinMode(OUTPUT_PIN, OUTPUT);
  int v_in = analogRead(INPUT_PIN);
  pTPS = v_in-MIN_TPS_IN;
  for (int i = 0; i < ( TPS_DIFF)/2; i++) {
    float x = (((float)i)/( TPS_DIFF)/2);
      float y = (((float)i)/( TPS_DIFF)/2);
    x = pow(x,POWERUP);
    x = x * (UPPER-LOWER);
    y = pow(y,POWERDOWN);
    y = y * (UPPER-LOWER);
    unsigned short res1 = ((unsigned short)x) + LOWER;
    unsigned short res2 = ((unsigned short)y) + LOWER;
    if (res1 > UPPER) res1 = UPPER;
    if (res1 < LOWER) res1 = LOWER; 
     if (res2 > UPPER) res2 = UPPER;
    if (res2 < LOWER) res2 = LOWER; 
  
    table_up[i] = res1;
    table_down[i] = res2;
#if PRINT
    Serial.print(i+MIN_TPS_IN);
    Serial.print(" ");
    Serial.println(table[i]);
#endif
  }
}

void loop() {
  int v_in = analogRead(INPUT_PIN); 
  int i_in = v_in - MIN_TPS_IN;
  int idx;
  if (pTPS - i_in > EXTRA) i_in = i_in + 150;
  else if (i_in - pTPS > EXTRA) i_in = i_in - 150;
  
  if (i_in < 0) idx = 0;
  else if (i_in > MAX_TPS_IN - MIN_TPS_IN) idx = MAX_TPS_IN-MIN_TPS_IN;
  else idx = i_in;

  int v_out;
  int movingAv = 0; // CHANGE THIS
  if (movingAv > 0)
  v_out = table_up[idx];
  else
  v_out = table_down[idx];
  //analogWrite(OUTPUT_PIN, v_out);
  dac.setVoltage(v_out, false);
  pTPS = i_in;
#if PRINT
  Serial.print(v_in);
  Serial.print(" ");
  Serial.println(v_out);
  delay(100);
#endif
}
