
#include <ioavr.h>

#define USE_PORTA
#define USE_PORTB
#define USE_PORTC

#include "iopins.h"
#include "pinlist.h"

using namespace IO;
typedef PinList<Pa1, Pa2, Pa3, Pa4, Pa5> Pins;

int main()
{
  Pins::SetConfiguration<Pins::Out, 0xff>();
  uint8_t i = 0;
  Pins::Write<0x05>();
  while(1)
  {
	Pins::Write(i++); 
  }	
}
