
#include "io430.h"
#include "../avrcpp/iopins.h"
#include "../avrcpp/pinlist.h"

using namespace IO;

void delay(unsigned count)
{
  for(unsigned i=0; i<count; i++)
    for(volatile unsigned j=0; j<1000; j++);
}

void Blink(int count)
{
    for(int i=0; i<count; i++)
    {
      delay(20);
      P1_0::Togle();
      delay(20);
      P1_6::Togle();
    }
}

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW | WDTHOLD;
  P1_0::SetDirWrite();
  P1_6::SetDirWrite();
  while(1)
  {    
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;  
    
    Blink(5);
    
    BCSCTL1 = CALBC1_8MHZ;                  
    DCOCTL = CALDCO_8MHZ;                   

     Blink(5*8);

    BCSCTL1 = CALBC1_12MHZ;                   
    DCOCTL = CALDCO_12MHZ;
    
    Blink(5*12);

    BCSCTL1 = CALBC1_16MHZ;                 
    DCOCTL = CALDCO_16MHZ;  
    
     Blink(5*16);

  }
  return 0;
}
