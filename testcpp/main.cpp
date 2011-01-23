
#include <avr/io.h>
#include <avr/interrupt.h>
#include "iopins.h"
#include "pinlist.h"
//#include "util.h"


using namespace IO;

typedef PinList<Pa5, Pa1, Pa6, Pa3, Pa7, Pa5, Pa0 >  Pins;

/*
int8_t floorLog2(uint32_t n) 
{
  if (n == 0)
    return -1;
 
  int pos = 0;
  if (n >= 1<<16) { n >>= 16; pos += 16; }
  if (n >= 1<< 8) { n >>=  8; pos +=  8; }
  if (n >= 1<< 4) { n >>=  4; pos +=  4; }
  if (n >= 1<< 2) { n >>=  2; pos +=  2; }
  if (n >= 1<< 1) {           pos +=  1; }
  return pos;
}
*/
/*
int8_t mylog2 (uint32_t val) {
    unsigned int ret = -1;
    while (val != 0) {
        val >>= 1;
        ret++;
    }
    return ret;
}
*/

//__attribute((noinline))
//void Foo(uint8_t *v)
//{
//	PORTA = *v;
//}

volatile uint8_t v=0;
__attribute((OS_main))
int main()
{
Pins::Write(v);
Pins::Set(v);
Pins::Clear(v);
while(1)
{}
}


