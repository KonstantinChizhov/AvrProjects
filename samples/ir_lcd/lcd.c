#include "lcd.h"


static void lcd_write4(uint8_t c) //4 msb
{
	LCD_PORT_DATA&=~LCD_DATA_MSK;
	LCD_PORT_DATA |= LCD_BIT1(c, 4)| LCD_BIT1(c, 5)| LCD_BIT1(c, 6) | LCD_BIT1(c, 7); 
	
}

void lcd_write(unsigned char c) 
{
	lcd_write4(c);
	LCD_STROBE; 
	lcd_write4(c<<4); 
	LCD_STROBE;
} 

void lcd_clear(void) 
{ 
	clrRS(); 
	lcd_write(0x01); //-- Clear the Display 
	lcd_write(0x02); //-- Home the display 
	_delay_ms(1);
} 
void lcd_goto(unsigned char pos) 
{ 
	clrRS(); 
	lcd_write(0x80+pos); //-- sets the DDRAM Address 
	_delay_ms(1);
} 

void lcd_home(void) 
{ 
	clrRS(); //-- write command 
	lcd_write(0x02); //-- Home Everything 
	_delay_ms(1);
} 

void lcd_puts(const char * s) 
{ 
	setRS(); // write characters 
	while(*s){
		lcd_write(*s++);
	} 
} 

 

void lcd_init(void) 
{ 
	LCD_DDR_DATA|=LCD_DATA_MSK;
	LCD_DDR_CTRL|=LCD_CTRL_MSK;
	clrRS(); // write control bytes 
	_delay_ms(150);
	lcd_write4(0x30); 
	LCD_STROBE;
	_delay_ms(5);
	LCD_STROBE; 
	_delay_ms(60);
	LCD_STROBE; 
	_delay_ms(5);
	lcd_write4(0x20); // set 4 bit mode 
	LCD_STROBE; 
	lcd_write(0x28); // 4 bit mode, 1/16 duty, 5x8 font 
	
	lcd_write(0x08); // display off 
	lcd_write(0x0E); // display on, blink curson on 
	lcd_write(0x06); // entry mode 

} 

inline void lcd_putch(char c) 
{ 
	setRS(); //- write characters 
	lcd_write(c); 
} 
