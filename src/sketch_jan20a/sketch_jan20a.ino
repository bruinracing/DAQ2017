int lastInterruptIndex = -1;
char InterruptLog[256] = {0};
unsigned long TimeLog[256] = {0};


void setup()
{
  Serial.begin(9600);
  Serial.println("i am 12 and wat is this");
  InitialiseIO();
  InitialiseInterrupt();
  Serial.println("Jonathan is blind");
}

void loop() {
  /* Nothing to do: the program jumps automatically to Interrupt Service Routine "blink"
   in case of a hardware interrupt  */
   int wut = 0;

   delay(1000);
   for( int i = 0; i < lastInterruptIndex; i++){
    Serial.println(InterruptLog[i], HEX);
   }

   wut = analogRead(A0);
   Serial.println(A0);
}  

void InitialiseIO(){
  /*pinMode(A0, INPUT);     // Pin A0 is input to which a switch is connected
  digitalWrite(A0, HIGH);   // Configure internal pull-up resistor
  pinMode(A1, INPUT);    // Pin A1 is input to which a switch is connected
  digitalWrite(A1, HIGH);   // Configure internal pull-up resistor
  pinMode(A2, INPUT);    // Pin A2 is input to which a switch is connected
  digitalWrite(A2, HIGH);      // Configure internal pull-up resistor
  */
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
    store += 0x1;
    Serial.println("A0");
  }

  if (digitalRead(A1)==0){
    store += 0x2;;
    Serial.println("A1");
  }

  if (digitalRead(A2)==0) {
    store += 0x4;
    Serial.println("A2");
  }

  if (store > 0x0){
    lastInterruptIndex++;
    InterruptLog[lastInterruptIndex] = store;
    TimeLog[lastInterruptIndex] = micros();
  }  
}

