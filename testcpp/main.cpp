#include "ioports.h"
//#include "HD44780.h"
#include "pinlist.h"
#include "iopins.h"

using namespace IO;

///typedef PinList<Pa1, Pa2, Pb0, Pb3, Pc6, Pc7> pins;
typedef PinList<Pa0, Pa1, Pa2, Pa3, Pa4, Pa5, Pa6, Pa7> pins;

int main()
{

	pins::Slice<0, 8>::Write(0xff);

	//pins::Write(0xff);
	
	//PORTC = pins::PinRead();
	while(1)
	{	
		//PORTC = pins::Read();
		//Porta::Pin<7>::Set(Portb::Pin<7>::IsSet());
	}
}


