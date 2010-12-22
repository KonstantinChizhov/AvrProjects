#include <ST/iostm32f10xxB.h>

#define USE_PORTA
#define USE_PORTC
#define USE_PORTB
#include <iopins.h>
#include <pinlist.h>
#include <spi.h>
#include <latch.h>

using namespace IO;
using namespace IoPrivate;

typedef IO::Pc8 led;

volatile short v = 0;
typedef SoftSpi<Pa0, Pa1, Pa3> Spi;

typedef ThreePinLatch<Pa0, Pa1, Pa3, 'L'> Latch;

typedef TPin<Latch , 0> L0;
typedef TPin<Latch , 1> L1;
typedef PinList<Pa0, Pa1, Pa3, Pc1, L1, Pc15, Pb1> Pins;

int main()
{
  Pins::SetConfiguration(Pins::Out);
  Pins::Write(0xff);
  
//  Spi::ReadWrite(0xff); 
  //Portc::DirSet(0xffff);
  //Portc::DirSet(0x0);
    //Portc::DirWrite(0xf);
	 //Portc::DirWrite(0x0);
  //Portc::DirClearAndSet(0x0ff0, 0x0f00);
  //Portc::SetConfiguration(0x0ff0, Portc::Out);
	Portc::SetConfiguration(0x0ff0, Portc::Out);
	Porta::SetConfiguration(0x0ff0, Porta::Out);

  led::SetDirWrite();
  led::SetConfiguration(led::Port::AltOut);
  
  led::Set();  
  //Pins::DirSet(0xff);
  //Pins::Write(0x1f);
 // Pins::DirClear(0xff);
  //Pins::DirWrite(0xff);
  
  return 0;
}
