#include <SPI.h>
#include "FastLED.h"

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
#define POS_ADDR      2 // led begin
#define POS_DATA      3 // led end
#define POS_DATA_1    4 // led r
#define POS_DATA_2    5 // led g
#define POS_DATA_3    6 // led b

#define START         0xff

#define CMD_RESET     0x0
#define CMD_SET_DATA  0x1
#define CMD_GET_DATA  0x2
#define CMD_SET_MODE  0x3
#define CMD_SET_LED   0x4
#define NUM_LEDS  60
#define DATA_PIN  7

CRGB leds[NUM_LEDS];
uint8_t led_sem = 0;

byte buffer[8];
int position;
int outputs[10] = {PD0, PD1, PD2, PD3, PD4, PD5, PD7, PC0, PC1, PC2};
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
  for (int i = 0; i < sizeof(outputs) / sizeof(int); i++) {
    pinMode(outputs[i], OUTPUT);
    digitalWrite(outputs[i], LOW);
  }
  for (int i = 0; i < sizeof(inputs) / sizeof(int); i++) {
    pinMode(inputs[i], INPUT);
  }

  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
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
    if( pin_pos >= 0 && pin_pos < sizeof(inputs) /sizeof(int)) {
      SPDR = values[pin_pos];
    }
  } else if(position == POS_DATA && buffer[POS_COMMAND] == CMD_SET_DATA) {
    pin_pos = buffer[POS_ADDR];
    if( pin_pos >= 0 && pin_pos < sizeof(outputs) /sizeof(int) ) {
      pin_id = outputs[pin_pos];
      value = buffer[POS_DATA] == 0 ? LOW : HIGH;
      digitalWrite(pin_id, value);
    }
  } else if(position == POS_DATA_3 && buffer[POS_COMMAND] == CMD_SET_LED) {
    if(led_sem == 0) {
      led_sem = 1;
      uint8_t led_begin = buffer[POS_ADDR];
      uint8_t led_end = buffer[POS_DATA];
      uint8_t g = buffer[POS_DATA_1];
      uint8_t r = buffer[POS_DATA_2];
      uint8_t b = buffer[POS_DATA_3];
      for(int i = led_begin; i <= led_end; i++) {
        leds[i].red = r;
        leds[i].green = g;
        leds[i].blue = b;
      }  
      FastLED.show(); 
      led_sem = 0;
    }
  }
  position++;
}  // end of interrupt service routine (ISR) for SPI

void loop () {
  for (int i = 0; i < sizeof(inputs) / sizeof(int); i++) {
    if(input_types[i] == TYPE_DIGITAL) {
      values[i] = digitalRead(inputs[i]) == LOW ? 0x00 : 0x01;
    } else if(input_types[i] == TYPE_ANALOG) { 
      values[i] = (analogRead(inputs[i]) >> 2);     
    }
  }
  delay(100);
}
