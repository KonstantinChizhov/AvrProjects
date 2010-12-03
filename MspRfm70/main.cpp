
#include "io430.h"

#include <iopins.h>

#include <spi.h>
#include <Rfm70.h>
typedef IO::P1_6 DebugPin;

typedef SoftSpi<IO::P1_3, IO::P1_4, IO::P1_2> Spi;

typedef Rfm70<Spi, IO::P1_1, IO::P1_0, IO::P1_5> Transiver1;
const uint8_t buffer[32] = {'H', 'e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd', '!', ' ', 'F', 'r', 'o', 'm', ' ', 'R', 'f', 'm', '7', '0' };

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  DebugPin::SetDirWrite();
  WDTCTL = WDTPW + WDTHOLD;
  Transiver1::Init();

  Transiver1::SwitchToTxMode();

  
  while(1)
  {
    Transiver1::ClearInterruptStatus();
    Transiver1::Write(buffer, 32);
    if(Transiver1::Status() & TxDataSent)
      DebugPin::Set();
    else
      DebugPin::Clear();
    delay(10000);
  }
  return 0;
}
