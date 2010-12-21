#include <ST/iostm32f10xxB.h>

#define USE_PORTA
#define USE_PORTC
#define USE_PORTB
#include <iopins.h>
#include <pinlist.h>
#include <spi.h>

using namespace IO;

typedef IO::Pc8 led;
//typedef PinList<Pa0, Pa1, Pa3, Pc1, Pc15> Pins;
volatile short v = 0;
typedef SoftSpi<Pa0, Pa1, Pa3> Spi;

int main()
{
//  Spi::ReadWrite(0xff); 
  //Portc::DirSet(0xffff);
  //Portc::DirSet(0x0);
    //Portc::DirWrite(0xf);
	 //Portc::DirWrite(0x0);
  //Portc::DirClearAndSet(0x0ff0, 0x0f00);
  //Portc::SetConfiguration(0x0ff0, Portc::Out);
	Portc::SetConfiguration2(0x0ff0, Portc::Out);
	 Porta::SetConfiguration2(0x0ff0, Porta::Out);
  //Porta::SetPinConfiguration<1>(Porta::Out);
  led::SetDirWrite();
 
  led::Set();  
  //Pins::DirSet(0xff);
  //Pins::Write(0x1f);
 // Pins::DirClear(0xff);
  //Pins::DirWrite(0xff);
  
  return 0;
}
