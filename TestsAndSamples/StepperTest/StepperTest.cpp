
#include "stepper.h"
#include <avr/io.h>
#include "iopins.h"
#include "pinlist.h"
#include <util/delay.h>

using namespace IO;
SimpleStepper<PinList<Pa6, Pa1, Pa2, Pa3, Pa4, Pa5> > stepper;

int main()
{
	stepper.Enable();
	while(1)
	{
		stepper.StepFwd();
		_delay_ms(3);
	}	
}
