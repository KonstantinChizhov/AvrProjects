#include "stepper.h"
#include "ports.h"
#include <util/delay.h>

#if 0
SimpleStepper
<
	StepperPerPinOutput
	<
		TPin<Portc, 4>,	//in1
		TPin<Portc, 5>,	//in2
		TPin<Portc, 6>,	//in3
		TPin<Portc, 7>	//in4
	>,
	TPin<Portd, 7>,	//e1
	TPin<Portd, 6>	//e2
> stepper;

#else

SimpleStepper
<
	StepperPerPortOutput
	<
		Portc, 
		4,	//in1
		5,	//in2
		6,	//in3
		7	//in4
	>,
	TPin<Portd, 7>,	//e1
	TPin<Portd, 6>	//e2
> stepper;

#endif

int main()
{
	stepper.Enable();
	while(1)
	{
		stepper.StepFwd();
		_delay_ms(3);
	}	
}
