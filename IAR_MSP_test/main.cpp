
#include "io430.h"
#include "../avrcpp/pinlist.h"
#include "../avrcpp/iopins.h"
using namespace IO;

typedef PinList<P1_0, P1_1, P1_2, P1_3> Pins;

int main( void )
{
  Pins::Write(Port2::Read());
  return 0;
}
