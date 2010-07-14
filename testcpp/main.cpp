
#include "iopins.h"
#include "pinlist.h"
#include <avr/delay.h>
#include "LedDisplay.h"

using namespace IO;

typedef PinList<Pa1, Pa2, Pb4, Pb5, Pc0, Pa0, Pa3> MyPins;


int main()
{

	MyPins::DirWrite(0xff);
	while(1)
	{	
		MyPins::Write(0x55);
	}
}


