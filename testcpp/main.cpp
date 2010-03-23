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
 
	//typedef MakeTypelist<PW<Pa0,0>, PW<Pa7,1>, PW<Pa5,2>, PW<Pa2, 3>, PW<Pc3,4>, PW<Pc1,5>, PW<Pc2,6>, PW<Pd5, 7> >::Result pins;
	typedef MakeTypelist<PW<Pa0,0>, PW<Pa1,1>, PW<Pa2,2>, PW<Pa3, 3>, PW<Pa4,4>, PW<Pa5,5>, PW<Pa6,6>, PW<Pa7, 7> >::Result pins;

	PinList<pins>::Write(PORTB);
	while(1)
	{	
	
	}

}


