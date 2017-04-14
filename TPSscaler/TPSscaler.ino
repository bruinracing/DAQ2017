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
#define POWER 0.333

//0.52-4.7 -> 3-235
//0.55-3.9 -> 3-195

Adafruit_MCP4725 dac;
static unsigned short table[MAX_TPS_IN-MIN_TPS_IN+1];

#define PRINT 0

void setup() {
#if PRINT
  Serial.begin(115200);
#endif
  pinMode(INPUT_PIN, INPUT);
  dac.begin(0x62);
  //pinMode(OUTPUT_PIN, OUTPUT);
  for (int i = 0; i < MAX_TPS_IN-MIN_TPS_IN+1; i++) {
    float x = (((float)i)/(MAX_TPS_IN-MIN_TPS_IN+1));
    x = pow(x,POWER);
    x = x * (UPPER-LOWER);
    unsigned short res = ((unsigned short)x) + LOWER;
    if (res > UPPER) res = UPPER;
    if (res < LOWER) res = LOWER; 
    table[i] = res;
#if PRINT
    Serial.print(i+MIN_TPS_IN);
    Serial.print(" ");
    Serial.println(table[i]);
#endif
  }
}

void loop() {
  int v_in = analogRead(INPUT_PIN);
  int idx = v_in-MIN_TPS_IN;
  //idx = idx + (random(10)-5);
  if (idx < 0) idx = 0;
  if (idx > MAX_TPS_IN-MIN_TPS_IN) idx = MAX_TPS_IN-MIN_TPS_IN;
  int v_out = table[idx];
  //analogWrite(OUTPUT_PIN, v_out);
  dac.setVoltage(v_out, false);
#if PRINT
  Serial.print(v_in);
  Serial.print(" ");
  Serial.println(v_out);
  delay(100);
#endif
}
