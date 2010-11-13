
#include "io430.h"
#include "../mcucpp/pinlist.h"
#include "../mcucpp/iopins.h"
#include "../mcucpp/timer.h"

using namespace IO;
using namespace Timers;

typedef P1_0 Green;
typedef P1_6 Red;
typedef Timers::TimerFreqSetup<Timers::Timer0, 1000, 16000000> T0Setup;

void Delay(unsigned count)
{
  for(volatile unsigned j = 0; j<count; j++)
    for(volatile unsigned i = 0; i<1000; i++);
}

int main( void )
{
	Timer0::Start(T0Setup::Divider);
	Timer0::OutputCompare<0>::Set(T0Setup::ReloadValue);
	Timer0::SetMode(Timer0::ClearOnMatch0);
  
  while(1)
  {
   // Green::Togle();
   // Delay(200);
   // Red::Togle();
   // Port1::Togle(0xff);
    Delay(200);
  }
  
  return 0;
}
