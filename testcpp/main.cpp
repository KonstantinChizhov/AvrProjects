#include "ioports.h"
#include "HD44780.h"
#include "pinlist.h"
#include "iopins.h"

using namespace IO;

typedef PinList<Pa1, Pa2, Pb0, Pb3, Pc5, Pc6, Pc7> pins;
//typedef PinList<Pa0, Pa1, Pa2, Pa3, Pa4, Pa5, Pa6> pins;
typedef Lcd<pins> Lcd1;

int main()
{

	Lcd1::Init();
	while(1)
	{	
		
	}
}


