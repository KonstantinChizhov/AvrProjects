#include <avr/io.h>
#include "dispatcher.h"
#include "HD44780.h"


Lcd
<
	TPin<Porta, 6>,
	TPin<Porta, 5>,
	TPin<Porta, 4>,
	TPin<Porta, 3>,
	TPin<Porta, 2>,
	TPin<Porta, 1>,
	TPin<Porta, 0>
> lcd;


int main()
{
 	lcd.Puts("1234567890abcdef", 16);

	while(1)
	{
	
	}

}
