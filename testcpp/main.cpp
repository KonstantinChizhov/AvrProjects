
#include "iopins.h"
#include "pinlist.h"
#include "util.h"

using namespace IO;

typedef PinList<Pa1, Pa2, Pa3, Pb3, Pb4, Pb5> pins;

int main()
{
	pins::Write(PORTC);
}


