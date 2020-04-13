#include <SPI.h>
void setup() {
  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);

  // turn on SPI in slave mode
  SPCR |= _BV(SPE);

  // turn on interrupts
  SPI.attachInterrupt();
  pinMode(PD1, OUTPUT);
  pinMode(PD2, OUTPUT);
  pinMode(PD3, OUTPUT);
  pinMode(PD4, OUTPUT);
  pinMode(PD5, OUTPUT);
  pinMode(PD6, OUTPUT);
  pinMode(PD7, OUTPUT);
  pinMode(PB0, OUTPUT);
  pinMode(PB6, OUTPUT);
  pinMode(PB7, OUTPUT);
  pinMode(PC0, OUTPUT);
  pinMode(PC1, OUTPUT);
  pinMode(PC2, OUTPUT);
  pinMode(PC3, OUTPUT);
  pinMode(A6, INPUT);
  pinMode(A7, INPUT);
}

// SPI interrupt routine
ISR (SPI_STC_vect)
{
  byte c = SPDR;
  if(c == 0x01) {
    digitalWrite(PD1, HIGH);
    digitalWrite(PD2, HIGH);
    digitalWrite(PD3, HIGH);
    digitalWrite(PD4, HIGH);
    digitalWrite(PD5, HIGH);
    digitalWrite(PD6, HIGH);
    digitalWrite(PD7, HIGH);
    digitalWrite(PB0, HIGH);
    digitalWrite(PB6, HIGH);
    digitalWrite(PB7, HIGH);
    digitalWrite(PC0, HIGH);
    digitalWrite(PC1, HIGH);
    digitalWrite(PC2, HIGH);
    digitalWrite(PC3, HIGH);
  } else if (c == 0x00){
    digitalWrite(PD1, LOW);
    digitalWrite(PD2, LOW);
    digitalWrite(PD3, LOW);
    digitalWrite(PD4, LOW);
    digitalWrite(PD5, LOW);
    digitalWrite(PD6, LOW);
    digitalWrite(PD7, LOW);
    digitalWrite(PB0, LOW);
    digitalWrite(PB6, LOW);
    digitalWrite(PB7, LOW);
    digitalWrite(PC0, LOW);
    digitalWrite(PC1, LOW);
    digitalWrite(PC2, LOW);
    digitalWrite(PC3, LOW);
  }
 
  SPDR = (analogRead(A2)/4);
}  // end of interrupt service routine (ISR) for SPI

void loop () { }
