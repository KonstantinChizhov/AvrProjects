
#include <avr/io.h>
#include "iopins.h"
#include "pinlist.h"
#include "util.h"

using namespace IO;

typedef PinList<Pa1, Pa2, Pa3, Pa5, Pa6, Pb2, Pb4, Pb6>  Pins;

int main()
{
	Pins::DirWrite(PORTC);
  Pins::Write(PORTC);
  Pins::Write(PORTC);
  Pins::Write(PORTC);
  Pins::Write(PORTC);
}


