
#include <io.h>

#include "pinlist.h"
#include "iopins.h"

using namespace IO;

typedef PinList<P1_0, P1_1, P1_2, P1_3> Pins;
//class

int main()
{
    Pins::Write(Port2::Read());
    return 0;
}
