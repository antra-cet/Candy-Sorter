 #include <Servo.h>

Servo discServo;
int discPosition;
int timer0_compare;
int seconds4 = 0;

#define LED 13

void setup() {

  // Set the pin for the disc servomotor
  discServo.attach(9);
  discPosition = 0;
  Serial.begin(9600);
  pinMode(LED, OUTPUT);

  // Setup the TIMER1
  noInterrupts();
  TCCR0A = 0;
  TCCR0B = 0;   // reset TCCR1A and TCCR1B
  timer0_compare = 261120;
  TCNT0 = timer0_compare;
  TCCR0B |= (1 << CS02);   // Set prescaler to 1024
  TCCR0B |= (1 << CS00);
  TIMSK0 |= (1 << OCIE0A);   // Enable CTC
  interrupts();
}

void loop() {
}

ISR(TIMER0_COMPA_vect) {
  TCNT0 = timer0_compare;
  digitalWrite(LED, digitalRead(LED) ^ 1);
  Serial.println(discPosition);
  if (seconds4 > 1000) {
    seconds4 = 0;
  }
  
  if (seconds4 % 200 == 0) {
    discPosition = (discPosition + 90) % 270;
    discServo.write(discPosition); 
  }

   seconds4 = seconds4 + 1;
}
