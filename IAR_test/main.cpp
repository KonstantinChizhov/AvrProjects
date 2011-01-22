
#include <ioavr.h>

#define USE_PORTA
#define USE_PORTB
#define USE_PORTC

#include "iopins.h"
#include "pinlist.h"

using namespace IO;
typedef PinList<Pa1, Pa2, Pa3, Pa4, Pa5, Pa6>  Pins;
volatile uint8_t v=0;
int main()
{
  Pins::Write(v);
Pins::Set(v);
Pins::Clear(v);
  while(1)
  {
	
  }	
}
