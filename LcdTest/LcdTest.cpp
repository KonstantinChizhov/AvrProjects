
#include "HD44780.h"
#include "iopins.h"
#include "pinlist.h"

using namespace IO;

typedef Lcd<PinList<Pa0, Pa1, Pa2, Pa3, Pa4, Pa5, Pa6> > Lcd1;

int main()
{
	Lcd1::Init();
 	Lcd1::Puts("1234567890abcdef", 16);
	while(1)
	{	
	
	}

}
