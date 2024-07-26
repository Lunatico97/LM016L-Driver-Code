
#include "LCDriver_Diwas.h"

byte custom1[8] = {0x00, 0x02, 0x05, 0x05, 0x07, 0x07, 0x07, 0x00} ;
byte custom2[8] = {0x08, 0x14, 0x14, 0x04, 0x07, 0x07, 0x07, 0x00} ;

LCDiwas diwas(8,10,7,6,5,4) ;

void setup()
{
  diwas.lcd_init() ;
  diwas.lcd_loadCustomChar(custom1, CUSTOM_CH1) ;
  diwas.lcd_loadCustomChar(custom2, CUSTOM_CH2) ;
  diwas.lcd_writeStr("Lock!") ;
  diwas.lcd_setCursorPos(6,1) ;
  diwas.lcd_writeFloat(10.2868, 2, PREC_MAX) ;
  diwas.lcd_writeStr(" hi") ;
}

void loop()
{
  diwas.lcd_setCursorPos(4,2) ;
  diwas.lcd_write_data(CUSTOM_CH1) ;
  diwas.lcd_setCursorPos(4,2) ; 
  diwas.lcd_write_data(CUSTOM_CH2) ; 
}
