
//#include <io.h>

//#include "pinlist.h"
//#include "iopins.h"

//using namespace IO;

//typedef PinList<P1_0, P1_1, P1_2, P1_3> Pins;
//class

#define P1OUT ( *(volatile unsigned char *)0x0021 )

volatile unsigned char P1OUT2 asm("0x0021");
char *str = "12345678901234567890123456789012345678901234567890";
int main()
{
 //   Pins::Write(Port2::Read());

   // unsigned char v = (P1OUT2 ^ 0x0A) & 0x0f;
  //  P1OUT2 ^= v;
    for(int i=0; i<10; i++)
        P1OUT2 = str[i];
    return 0;
}
