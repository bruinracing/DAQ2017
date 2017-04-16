#define INPUTPIN 0
#define SAMPLES 4
#define SCALE 10
#define OUTPUTPIN 1
#define MAXOUT 1023


void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
  int dTPS = 0;
  int samples[SAMPLES];
  unsigned long timeSamples[SAMPLES];
  int startAvg = 0;
  int endAvg = 0;
  int startTime = 0;
  int endTime = 0;
  int outAvg = 0;

  //sample count is defined by SAMPLES.
  sample(samples, timeSamples); //this accumulates some samples of TPS signal with timestamps.

  // Note that after this point we assume that the array is populated with values!
  dTPScalc(startAvg, endAvg, startTime, endTime, dTPS, samples, timeSamples); //get the dTPS value

  if (dTPS > 0) //rescale the value only if it's increasing
  {
    rescale(outAvg, dTPS, startTime, startAvg);
  }
  else
  {
    //if we hit this, we need to decay towards the "true" TPS value or do nothing
    
  }
}

void sample(int (&samples)[SAMPLES], unsigned long (&timeSamples)[SAMPLES]) {
  for (int i = 0; i < SAMPLES; i++)
  {
    samples[i] = analogRead(INPUTPIN);
    timeSamples[i] = millis();
  }
}

void dTPScalc(int &startAvg, int &endAvg, int &startTime, int &endTime, int &dTPS, int (&samples)[SAMPLES], unsigned long (&timeSamples)[SAMPLES]) {
  int midCount = SAMPLES / 2;
  startAvg = 0;
  endAvg = 0;
  startTime = 0;
  endTime = 0;
  for (int i = 0; i < midCount; i++)
  {
    startAvg += samples[i];
    startTime += timeSamples[i];
  }
  startAvg /= midCount;
  startTime /= midCount;
  
  for (int i = midCount; i < SAMPLES; i++)
  {
    endAvg += samples[i];
    endTime += timeSamples[i];
  }
  endAvg /= midCount;
  endTime /= midCount;
  dTPS = (endAvg - startAvg) / (endTime - startTime);
}

void rescale(int &outAvg, int &dTPS, int &startTime, int &startAvg) {
  int targetdTPS = 10 * dTPS;
  int currentTime = millis();

  outAvg = (currentTime - startTime) * targetdTPS;
  outAvg += startAvg;

  if (outAvg > MAXOUT)
  {
    outAvg = MAXOUT;
  }
  analogWrite(OUTPUTPIN, outAvg / 4);
}

