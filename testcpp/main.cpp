#include "ioports.h"
//#include "HD44780.h"
#include "pinlist.h"
#include "iopins.h"

using namespace IO;

typedef PinList<Pa1, Pa2, Pb0, Pb3> pins;

int main()
{
	//pins::Write(0xff);
	
	//PORTC = pins::PinRead();
	while(1)
	{	
		//PORTC = pins::Read();
		Porta::Pin<7>::Set(Portb::Pin<7>::IsSet());
	}
}


