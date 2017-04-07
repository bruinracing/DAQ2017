#define INPUT_PIN A0
#define OUTPUT_PIN 6
#define LOWER 100

//0.52-4.7 -> 3-235
//0.55-3.9 -> 3-195

void setup() {
  Serial.begin(115200);
  pinMode(INPUT_PIN, INPUT);
  pinMode(OUTPUT_PIN, OUTPUT);
//  setPwmFrequency(6,1);
}

void loop() {
  int v_in = analogRead(INPUT_PIN);
  int v_out = map(v_in, 0, 800, LOWER, 240);
  if (v_out > 240) v_out = 240;
  if (v_out < LOWER) v_out = LOWER
  //Serial.print(v_in);
  //Serial.print(" ");
  //Serial.println(v_out);
  analogWrite(OUTPUT_PIN, v_out);
  //delay(100);
}
