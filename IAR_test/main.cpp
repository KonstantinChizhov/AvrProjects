
#include <ioavr.h>

#define USE_PORTA
#include "../avrcpp/iopins.h"
#include "../avrcpp/pinlist.h"

using namespace IO;
typedef PinList<Pa1, Pa2, Pa3, Pa5, Pa6> Pins;

int main()
{
  Pins::Write(0xff);
  return 0;
}
