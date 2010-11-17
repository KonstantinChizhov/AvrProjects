
#include <ioavr.h>

#define USE_PORTA
#include "../mcucpp/iopins.h"
#include "../mcucpp/pinlist.h"

using namespace IO;
typedef PinList<Pa1, Pa2, Pa3, Pa5, Pa6> Pins;

uint8_t x1, x2;
static volatile uint16_t EncData=0;
static volatile uint16_t EncData2=0;

void Enc2()
{
	uint8_t y1 , y2;
	y1 = PORTB;
	y2 = PORTA;

	uint8_t fwd  = ~(x1 | y2) & (x2 ^ y1) | x1 & y2 & ~(x2 & y1);
	uint8_t back = ~(x2 | y1) & (x1 ^ y2) | x2 & y1 & ~(x1 & y2);
	if(fwd&1)EncData++;
	if(back&1)EncData--;
	if(fwd&2)EncData2++;
	if(back&2)EncData2--;
	x1 = y1;
	x2 = y2;
}


int main()
{
  //Pins::Write(0xff);
  Enc2();
  return 0;
}
