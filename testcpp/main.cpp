//#include <avr/io.h>
//#include "dispatcher.h"
//#include "usart.h"
//#include "adc.h"
//#include "HD44780.h"
//#include "stepper.h"
//#include "LedDisplay.h"
//#include "util.h"
#include "ports.h"


int main()
{
 
	uint8_t v = PinList<PW<Pa0,0>, 
					PinList<PW<Pa1,1>, NullPin> >::MASK;// , , PW<Pa3,2>, PW<Pa4,3>;

PORTB=v;

	while(1)
	{	
	
	}

}


