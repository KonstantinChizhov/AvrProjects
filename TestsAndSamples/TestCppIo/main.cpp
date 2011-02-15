#include <avr/io.h>

#include "iopins.h"
#include "pinlist.h"

using namespace IO;

typedef PinList<Pa1, Pa0, Pa4, Pa6, Pa3, Pa2, Pa7, Pa5> pins;


volatile uint8_t value = 0x55;

int main()
{
	pins::SetConfiguration<pins::Out, 0xff>();
	pins::Set<0xff>();
	pins::Clear<0xff>();
	pins::Write<0xff>();

	pins::SetConfiguration(pins::Out);

	pins::Set(value);
	pins::Clear(value);
	pins::Write(value);

	value = pins::PinRead();
	
	while(1)
	{	
		
	}

}


