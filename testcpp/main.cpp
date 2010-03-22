//#include <avr/io.h>
//#include "dispatcher.h"
//#include "usart.h"
//#include "adc.h"
//#include "HD44780.h"
//#include "stepper.h"
//#include "LedDisplay.h"
//#include "util.h"
#include "ports.h"
#include "TypeList.h"

PinSet2<Pa1, Pa4> set;

int main()
{
 	TYPELIST_4(Pa1, Pa2, Pa3, Pa4) a;
 	set.Write(3);
	while(1)
	{	
	
	}

}


