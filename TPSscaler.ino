#define INPUT_PIN A0
#define OUTPUT_PIN 6

//bounds for the output - changing lower changes the output degree of 0 
//                      - changing upper changes the output degree of MAX_TPS  
#define LOWER 85
#define UPPER 220

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

#define MAX_TPS_IN 800
#define MIN_TPS_IN 100
#define POWER 0.333


//0.52-4.7 -> 3-235
//0.55-3.9 -> 3-195

unsigned char table[MAX_TPS_IN-MIN_TPS_IN+1];

void setup() {
  Serial.begin(115200);
  pinMode(INPUT_PIN, INPUT);
  pinMode(OUTPUT_PIN, OUTPUT);
  for (int i = 0; i < MAX_TPS_IN-MIN_TPS_IN+1; i++) {
    float x = (((float)i)/(MAX_TPS_IN-MIN_TPS_IN+1));
    //x = sqrt(x);
    x = pow(x,POWER);
    x = x * (UPPER-LOWER);
    unsigned char res = ((unsigned char)x) + LOWER;
    if (res > UPPER) res = UPPER;
    if (res < LOWER) res = LOWER; 
    table[i] = res;
    Serial.print(i+MIN_TPS_IN);
    Serial.print(" ");
    Serial.println(table[i]);
  }
}

void loop() {
  int v_in = analogRead(INPUT_PIN);
  int idx = v_in-MIN_TPS_IN;
  if (idx < 0) idx = 0;
  if (idx > MAX_TPS_IN-MIN_TPS_IN) idx = MAX_TPS_IN-MIN_TPS_IN;
  int v_out = table[idx];
  //int v_out = map(v_in, MIN_TPS_IN, MAX_TPS_IN, LOWER, UPPER);
  //if (v_out > UPPER) v_out = UPPER;
  //if (v_out < LOWER) v_out = LOWER;
  //Serial.print(v_in);
  //Serial.print(" ");
  //Serial.println(v_out);
  analogWrite(OUTPUT_PIN, v_out);
  //delay(200);
}
