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
 	typedef PinList<Pa0, Pa1, Pa2, Pa3, Pb5, Pb4, Pb2> pins;

	pins::Write(PORTC);

	while(1)
	{	
	
	}

}


