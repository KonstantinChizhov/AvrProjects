
#include <avr/io.h>
#include <avr/interrupt.h>
#include "iopins.h"
#include "pinlist.h"
#include "util.h"

using namespace IO;

typedef PinList<Pa1Inv, Pa2, Pa3, Pa6Inv, Pa5>  Pins;

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

__attribute((OS_main))
int main()
{

//	Porta::ClearAndSet(0xf, 0x03);

//Pa1Inv::Set();
//Pa1Inv::Set(false);

//Pa1::SetDirWrite();
//Porta::SetPinConfiguration<1>(Porta::Out);

	//uint8_t c = 10;
	//Foo(&c);
//	Pins::SetConfiguration(Pins::Out);
	Pins::Write<0x55>();
//	PORTB = Pins::PinRead();

	//PORTA = Bar(PORTB);

	return 0;
	/*Pins::DirWrite(PORTC);
  Pins::Write(PORTC);
  Pins::Write(PORTC);
  Pins::Write(PORTC);
  Pins::Write(PORTC);*/
	//PORTA = floorLog2(PORTB | PORTC << 8);
//	PORTA = ilog10(PORTB | PORTC << 8);

}


