#ifndef LCD_H
#define LCD_H

#include <avr/io.h>
#include <util/delay.h>

#define LCD_PORT_DATA PORTA
#define LCD_DDR_DATA DDRA

#define LCD_PORT_CTRL PORTA
#define LCD_DDR_CTRL DDRA

#define RS 2
//#define RW 1
#define E 3

#define D4 4
#define D5 5
#define D6 6
#define D7 7
 
#define LCD_DATA_MSK (_BV(D4)|_BV(D5)|_BV(D6)|_BV(D7))
#define LCD_CTRL_MSK (_BV(RS)|_BV(E))

#define LCD_BIT1(val, bit) ((val >> (bit))&1)<<(D##bit)

#define LCD_STROBE LCD_PORT_CTRL|=(1<<E); _delay_ms(0.1); LCD_PORT_CTRL&=(unsigned char)~(1<<E); _delay_ms(0.2);

#define setRS() LCD_PORT_CTRL|=(1<<RS)
#define clrRS() LCD_PORT_CTRL&=(unsigned char)~(1<<RS)

void lcd_write(unsigned char c);
void lcd_clear(void);
void lcd_goto(unsigned char pos);
void lcd_home(void); 
void lcd_puts(const char * s);
void lcd_init(void);
inline void lcd_putch(char c);


#endif
