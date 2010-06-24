#include "ioports.h"
#include "HD44780.h"
#include "pinlist.h"
#include "iopins.h"

using namespace IO;

typedef PinList<Pa1, Pa2, Pb0, Pb3> pins;

int main()
{
	pins::Write(0xff);
	while(1)
	{	
		
	}

}


