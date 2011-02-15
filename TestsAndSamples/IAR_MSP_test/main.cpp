
#include "io430.h"
#include "pinlist.h"
#include "iopins.h"
#include "timers.h"
#include "timer_utils.h"

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

typedef PinList<P1_0, P1_1, P1_2, P1_3, NullPin, P1_5> Pins;

int main( void )
{
	//Timer0::Start(T0Setup::Divider);
	//Timer0::OutputCompare<0>::Set(T0Setup::ReloadValue);
	//Timer0::SetMode(Timer0::ClearOnMatch0);
//  Green::SetConfiguration(Green::Port::Out);
  Pins::SetConfiguration<Pins::Out, 0xff>();
  
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
