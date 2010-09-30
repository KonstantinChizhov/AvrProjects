
#include <ioavr.h>

#define USE_PORTA
#include "../mcucpp/iopins.h"
#include "../mcucpp/pinlist.h"

using namespace IO;
typedef PinList<Pa1, Pa2, Pa3, Pa5, Pa6> Pins;

int main()
{
  Pins::Write(0xff);
  return 0;
}
