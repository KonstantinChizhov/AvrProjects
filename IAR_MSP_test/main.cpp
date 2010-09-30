
#include "io430.h"
#include "../mcucpp/pinlist.h"
#include "../mcucpp/iopins.h"
using namespace IO;

//typedef PinList<P1_0, P1_1, P1_2, P1_3> Pins;
typedef P1_0 Green;
typedef P1_6 Red;

void Delay(unsigned count)
{
  for(volatile unsigned j = 0; j<count; j++)
    for(volatile unsigned i = 0; i<1000; i++);
}

int main( void )
{
  //Pins::Write(Port2::Read());
  //Green::SetDirWrite();
  //Red::SetDirWrite();
  Port1::DirSet(0xff);
  while(1)
  {
   // Green::Togle();
   // Delay(200);
   // Red::Togle();
    Port1::Togle(0xff);
    Delay(200);
  }
  
  return 0;
}
