
#include "LCDriver_Diwas.h"

#define trigger 12
#define echo 13

LCDiwas diwas(2,3,7,6,5,4) ;
byte custom[8] = {0x04, 0x0E, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00} ;

long timeperiod ;
float distance ;
void setup()
{
  diwas.lcd_init() ;
  
  pinMode(trigger, OUTPUT) ;
  pinMode(echo, INPUT) ;

  diwas.lcd_enableUnderCursor() ;
  diwas.lcd_loadCustomChar(custom, CUSTOM_CH2) ;

  diwas.lcd_writeStr("Away: ") ;
  diwas.lcd_setCursorPos(11,1) ;
  diwas.lcd_writeStr(" cm") ;
  diwas.lcd_disableCursors() ;
}

void loop()
{
    digitalWrite(trigger, LOW) ;
    delayMicroseconds(2) ;
    digitalWrite(trigger, HIGH) ;
    delayMicroseconds(10) ;
    digitalWrite(trigger, LOW) ;
    timeperiod = pulseIn(echo, HIGH) ;
    distance = (timeperiod*0.0343)/2 ;
    distance = distance/100 ;
    
    diwas.lcd_setCursorPos(6, 1) ;
    diwas.lcd_writeFloat(distance, 2, 2) ;
    
    
    diwas.lcd_writeStr("") ;
    delay(2) ;
}
