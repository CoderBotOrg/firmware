#include <SPI.h>

/* 
 *  buffer is used to store input message.
 *  input message is always 4 bytes long with following format
 *  [0] - start of message
 *  [1] - command
 *  [2] - address
 *  [3] - data
 *  
 *  <command>:
 *  0 - no operations, reset buffer
 *  1 - set output state
 *  2 - read input state
 *  3 - set output mode
 *  
 *  <address>:
 *  0 .. 14 - digital input/output
 *  15 .. 16 - analog input
 *  
 *  <data1>:
 *  0 - set digital output to "low" (false)
 *  1 - set digital output to "high" (true)
 *  
 *  <data2>: 
 *  any value (unused)
 */

#define TYPE_DIGITAL  1
#define TYPE_ANALOG   2

#define POS_START     0
#define POS_COMMAND   1
#define POS_ADDR      2
#define POS_DATA      3

#define START         0xff

#define CMD_RESET     0x0
#define CMD_SET_DATA  0x1
#define CMD_GET_DATA  0x2
#define CMD_SET_MODE  0x3

byte buffer[8];
int position;
int outputs[11] = {PD0, PD1, PD2, PD3, PD4, PD5, PD6, PD7, PC0, PC1, PC2};
int inputs[6] = {A6, A7, PB0, PB6, PB7, PC3};
int values[6] = {0, 0, 0, 0, 0, 0};
int input_types[6] = {TYPE_ANALOG, TYPE_ANALOG, TYPE_DIGITAL, TYPE_DIGITAL, TYPE_DIGITAL, TYPE_DIGITAL};


void setup() {
  memset(buffer, 0, sizeof(buffer));
  position = 0;
  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);

  // turn on SPI in slave mode
  SPCR |= _BV(SPE);

  // turn on interrupts
  SPI.attachInterrupt();
  for (int i = 0; i < sizeof(outputs); i++) {
    pinMode(outputs[i], OUTPUT);
    digitalWrite(outputs[i], LOW);
  }
  for (int i = 0; i < sizeof(inputs); i++) {
    pinMode(inputs[i], INPUT);
  }
}

// SPI interrupt routine
ISR (SPI_STC_vect)
{
  byte c = SPDR;
  int pin_pos = 0;
  int pin_id = 0;
  byte value = 0;

  if(c == START) {
    position = 0;
  }
  buffer[position] = c;
  if(position == POS_ADDR && buffer[POS_COMMAND] == CMD_GET_DATA) {
    pin_pos = buffer[POS_ADDR];    ;
    if( pin_pos >= 0 && pin_pos < sizeof(inputs)) {
      SPDR = values[pin_pos];
    }
  } else if(position == POS_DATA && buffer[POS_COMMAND] == CMD_SET_DATA) {
    pin_pos = buffer[POS_ADDR];
    if( pin_pos >= 0 && pin_pos < sizeof(outputs)) {
      pin_id = outputs[pin_pos];
      value = buffer[POS_DATA] == 0 ? LOW : HIGH;
      digitalWrite(pin_id, value);
    }
  }
  position++;
}  // end of interrupt service routine (ISR) for SPI

void loop () {
  for (int i = 0; i < sizeof(inputs); i++) {
    if(input_types[i] == TYPE_DIGITAL) {
      values[i] = digitalRead(inputs[i]) == LOW ? 0x00 : 0x01;
    } else if(input_types[i] == TYPE_ANALOG) { 
      values[i] = (analogRead(inputs[i]) >> 2);     
    }
  }
  delay(100);
}
