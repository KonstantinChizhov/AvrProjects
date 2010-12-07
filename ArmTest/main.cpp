#include <ST/iostm32f10xxB.h>

#define USE_PORTA
#define USE_PORTC
#include <iopins.h>

typedef IO::Pc8 led;


int main()
{
  //led::SetDirWrite();
  GPIOA_CRH_bit.MODE8 = 1;
  led::Set();  
  return 0;
}
