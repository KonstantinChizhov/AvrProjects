#include <ST/iostm32f10xxB.h>

#define USE_PORTA
#define USE_PORTC
#include <iopins.h>
#include <pinlist.h>

using namespace IO;

typedef IO::Pc8 led;
typedef PinList<Pa0, Pa1, Pa3, Pc1, Pc15> Pins;
volatile short v = 0;
int main()
{
  //led::SetDirWrite();
 
  //led::Set();  
  Pins::DirSet(0xff);
  Pins::Write(0x1f);
  Pins::DirClear(0xff);
  Pins::DirWrite(0xff);
  return 0;
}
