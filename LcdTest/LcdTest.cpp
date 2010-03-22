
#include "HD44780.h"
#include "ports.h"

#if 0
Lcd
<
	TPin<Porta, 6>,
	TPin<Porta, 5>,
	TPin<Porta, 4>,
	PinsLcdDataBus
	<
		TPin<Porta, 3>,
		TPin<Porta, 2>,
		TPin<Porta, 1>,
		TPin<Porta, 0>
	>
> lcd;

#else

Lcd
<
	TPin<Porta, 6>,
	TPin<Porta, 5>,
	TPin<Porta, 4>,
	PortLcdDataBus
	<
		Porta, 0
	>
> lcd;

#endif

int main()
{
 	lcd.Puts("1234567890abcdef", 16);
	while(1)
	{	
	
	}

}
