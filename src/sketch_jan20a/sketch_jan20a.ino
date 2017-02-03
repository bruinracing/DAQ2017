#include <avr/pgmspace.h>

int lastInterruptIndex = -1;
int headIntIndex = -1;
char InterruptLog[256] = {0};
unsigned long TimeLog[256] = {0};
bool ramOverflow = false;

void setup()
{
  Serial.begin(9600);
  Serial.println("i am 12 and wat is this");
  InitialiseIO();
  InitialiseInterrupt();
}

void loop() {
   delay(500);
   if (ramOverflow){
    Serial.println("Overflowed SRAM buffer!");
    ramOverflow = false; //reset, this is not strictly unrecoverable.
   }
   if (lastInterruptIndex < 0){
    return; //restart the loop don't do unneccessary work
   }
   int A0 = 0;
   int A1 = 0;
   int A2 = 0;
   for( int i = headIntIndex; i < lastInterruptIndex; i++){
    char toTwiddle = InterruptLog[i];
    char temp0 = toTwiddle & 0x1;
    char temp1 = (toTwiddle & 0x2) >> 1;
    char temp2 = (toTwiddle & 0x4) >> 2;
    A0 += temp0;
    A1 += temp1;
    A2 += temp2;
   }
   unsigned long timeElapsed = TimeLog[lastInterruptIndex] - TimeLog[0];
   float avg0 = A0/timeElapsed;
   float avg1 = A1/timeElapsed;
   float avg2 = A2/timeElapsed;
   Serial.println(avg0, 2);
   Serial.println(avg1, 2);
   Serial.println(avg2, 2);
   return;
}  

void InitialiseIO(){
  pinMode(A0, INPUT);     // Pin A0 is input to which a switch is connected
  digitalWrite(A0, HIGH);   // Configure internal pull-up resistor
  pinMode(A1, INPUT);    // Pin A1 is input to which a switch is connected
  digitalWrite(A1, HIGH);   // Configure internal pull-up resistor
  pinMode(A2, INPUT);    // Pin A2 is input to which a switch is connected
  digitalWrite(A2, HIGH);      // Configure internal pull-up resistor
}

void InitialiseInterrupt(){
  cli();    // switch interrupts off while messing with their settings  
  PCICR =0x02;          // Enable PCINT1 interrupt
  PCMSK1 = 0b00000111;
  sei();    // turn interrupts back on
}

ISR(PCINT1_vect) {    // Interrupt service routine. Every single PCINT8..14 (=ADC0..5) change
            // will generate an interrupt: but this will always be the same interrupt routine
  volatile char store = 0x0;

  if (digitalRead(A0)==0){
    store |= 0x1;
    Serial.println("A0");
  }

  if (digitalRead(A1)==0){
    store |= 0x2;;
    Serial.println("A1");
  }

  if (digitalRead(A2)==0) {
    store |= 0x4;
    Serial.println("A2");
  }

  if (store > 0x0){
    if (lastInterruptIndex >= 255){
       lastInterruptIndex = -1; //if we are about to overflow buffer, overwrite oldest.
       ramOverflow = true;
    }
    lastInterruptIndex++;
    InterruptLog[lastInterruptIndex] = store;
    TimeLog[lastInterruptIndex] = micros();
  }  
}

