#include "stepper.h"
#include "ports.h"
#include <util/delay.h>

//SimpleStepper<PinList<Pa6, Pa1, Pa2, Pa3, Pa4, Pa5> > stepper;
/*
void f1()
{
	stepper.StepBack();
}

void f3()
{
	stepper.StepFwd();
}
*/

int main()
{
	using namespace IO;
	typedef PinList<Pa1, Pa2, Pa3, Pa4> p; 
	p::Write(PORTC);
	//PORTC = p::Read();
	//stepper.Enable();
	while(1)
	{
		//stepper.StepFwd();
		//_delay_ms(3);
	}	
}
