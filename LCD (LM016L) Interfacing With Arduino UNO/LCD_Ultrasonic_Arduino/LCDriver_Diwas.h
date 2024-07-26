//LCD Driver for LM160L  
//Author: Diwas Adhikari
//For those who don't want to download <LiquidCrystal.h> library and use LCD (WARNING : 16*2 only) display

//Please read the instruction file to learn the instruction set of LM160L !

//Only 8 custom characters are available to store in CGRAM
#define CUSTOM_CH1 0x00 
#define CUSTOM_CH2 0x01 
#define CUSTOM_CH3 0x02 
#define CUSTOM_CH4 0x03
#define CUSTOM_CH5 0x04 
#define CUSTOM_CH6 0x05 
#define CUSTOM_CH7 0x06 
#define CUSTOM_CH8 0x07
//Starting Addresses of DDRAM (1 A A A A A A A) and CGRAM(0 1 A A A A A A)
#define DDRAM_START 0x80
#define CGRAM_START 0x40
//Precision while printing floats
#define PREC_MAX 4
#define PREC_NONE 0
//Default Sets
#define CLR 0x01 (0 0 0 0 0 0 0 1)
#define HOME 0x02 (0 0 0 0 0 0 1 0) 
#define CURS_SET 0x10 // (0 0 0 1 S/C R/L X X) Cursor to right
#define FUNC_SET 0x20 // (0 0 1 DL N F X X) 4 bit mode with 1 line and 5*8 font
#define ENT_SET  0x04 // (0 0 0 0 0 1 I/D S) Entry increment to right and shift disabled
#define DIS_SET  0x08 // (0 0 0 0 1 D C B) Display on with cursors disabled
//Offsets for function set
#define LINE_1 0x00 
#define LINE_2 0x08
#define BIT_4  0x00
#define BIT_8  0x10
#define FONT_8 0x00
#define FONT_10 0x04
//Offsets for display set
#define CURS_ON 0x02
#define CURS_OFF 0x00
#define BL_ON 0x01
#define BL_OFF 0x00
#define DIS_ON 0x00
#define DIS_OFF 0x04

class LCDiwas{

private : 
	      int RS,D4, D5, D6, D7, E ;
        uint8_t offsets[2] = {0x00, 0x40} ;
  
public :

       LCDiwas(int enable, int rs, int d7, int d6, int d5, int d4){
          E = enable ; RS = rs ; D7 = d7 ; D6 = d6 ; D5 = d5 ; D4 = d4 ;
          pinMode(E, OUTPUT) ;
          pinMode(RS, OUTPUT) ;
          pinMode(D7, OUTPUT) ;
          pinMode(D6, OUTPUT) ;
          pinMode(D5, OUTPUT) ;
          pinMode(D4, OUTPUT) ;
      }

      void lcd_write_4bit(int registerSelect, char character)
      {
          digitalWrite(RS, registerSelect) ;
          digitalWrite(D4, (character & 0b00010000) >> 4) ;
          digitalWrite(D5, (character & 0b00100000) >> 5) ;
          digitalWrite(D6, (character & 0b01000000) >> 6) ;
          digitalWrite(D7, (character & 0b10000000) >> 7) ;
          delay(15) ;
          digitalWrite(E, HIGH) ;
          delay(15) ;
          digitalWrite(E, LOW) ;
          delay(15) ;
          digitalWrite(D4, (character & 0b00000001) >> 0) ;
          digitalWrite(D5, (character & 0b00000010) >> 1) ;
          digitalWrite(D6, (character & 0b00000100) >> 2) ;
          digitalWrite(D7, (character & 0b00001000) >> 3) ;
          delay(15) ;
          digitalWrite(E, HIGH) ;
          delay(15) ;
          digitalWrite(E, LOW) ;
          delay(15) ;
       }

       //For writing commands 
        void lcd_write_cmd(char cmd_hex){
          lcd_write_4bit(0, cmd_hex) ;
        }
        
        //For writing data to display characters
        void lcd_write_data(char data){
          lcd_write_4bit(1, data) ;
        }

        void lcd_clear()
        {
          lcd_write_cmd(0x01) ;
          delay(15) ;
        }

        void lcd_home()
        {
          lcd_write_cmd(0x02) ;
          delay(15) ;  
        }

        void lcd_enableUnderCursor()
        {
          lcd_write_cmd(0x0E) ;
        }

        void lcd_enableBlinkCursor()
        {
          lcd_write_cmd(0x0D) ;
        }

        void lcd_disableCursors()
        {
          lcd_write_cmd(0x0C) ;  
        }

        void lcd_enableCursors()
        {
          lcd_write_cmd(0x0F) ;  
        }

        void lcd_animToRight()
        {
          lcd_write_cmd(0x06) ;  
        }
        
        void lcd_setCursorPos(uint8_t col, uint8_t row)
        {
          lcd_write_cmd(DDRAM_START | (col + offsets[row-1]) ) ; //DDRAM  
        }
        
        void lcd_loadCustomChar(byte ch[], uint8_t charNum)
        {
          charNum &= 0x07 ;
	        lcd_write_cmd(CGRAM_START | (charNum << 3)) ; //CGRAM
          for(int i=0; i<8; i++){
              lcd_write_data(ch[i]) ;
          }
          lcd_clear() ;
        }
  
        //Initialize LCD Display
        void lcd_init(){
    
          //Wake Up Calls
          lcd_write_cmd(0x33) ; //8-bit mode default at first
          delay(15) ; //Always delay 2ms after first command
          lcd_write_cmd(0x32) ; //8-bit mode
          delay(15) ;

          // Function Set
          lcd_write_cmd(0x28) ; 
          delay(15) ;

          //Clear
          lcd_write_cmd(0x01) ;
          delay(15) ; 
          
          // Display Set 
          lcd_write_cmd(0x0C) ; 
          delay(15) ;
        }

        void lcd_writeStr(char str[])
        { 
             int count = 0 ;
             for(int i=0; str[i] != 0; i++)
                  count++ ;
             for(int i=0; i<count; i++) 
               lcd_write_data(str[i]) ;
        }

/*If you are writing numbers along with custom characters loaded and the buffer you set for number has number of lesser digits,
  empty buffer would have 0x00 value that executes CUSTOM_CH1 so, avoid CUSTOM_CH1 and pick any higher address(CUSTOM_CH2 or greater)
  to use both custom characters and write numbers/floats. */
        void lcd_writeNum(int num, int buffer_size)
        { 
             char *str = new char[buffer_size] ;
             itoa(num, str, 10) ;
             for(int i=0; i<buffer_size; i++)
               lcd_write_data(str[i]);
             delete str ;
        }

/*If you want to just print numbers without decimals, lcd_writeNum() is enough and better as, it is faster but you can set 
  the precision of lcd_writeFloat() to PREC_NONE or, 0 to write integers as well. */
        void lcd_writeFloat(float num, int buffer_size, int prec)
        { 
             char *intStr = new char[buffer_size] ;
             char *precStr = new char[prec] ;
             int temp = int(num) ;
             float diff ;
             diff = abs(num-temp) ;
             diff *= 10000 ;
             itoa(temp, intStr, 10) ;
             itoa(diff, precStr, 10) ;
             for(int i=0; i<buffer_size; i++)
               lcd_write_data(intStr[i]);
             if(prec>0) lcd_write_data('.') ;
             for(int i=0; i<prec; i++)
               lcd_write_data(precStr[i]);
             delete intStr ;
             delete precStr ;
        }
} ;
