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

static void delay_loop(unsigned long count)
{
  do{}while(count--);
}

inline void
_delay_ms(unsigned __ms)
{
  delay_loop(__ms*1000);
}

inline void
_delay_us(unsigned __us)
{
  delay_loop(__us);
}


#include "HD44780.h"

using namespace IO;
typedef PinList<Pa0, Pa1, Pa2, Pa3, Pa4, Pa5, Pa6> LcdBus;
typedef Lcd<LcdBus> MyLcd;

int main()
{
   MyLcd::Init();
  MyLcd::Puts("Hello msp430", 8);
  MyLcd::Goto(0x40);
  MyLcd::Puts("Hello msp430"+8, 4);
  
 // Pins::SetConfiguration(Pins::Out, 0xff);
  //Pins::Write(0x0f);
 
 // Spi::ReadWrite(0xff); 
  //Portc::DirSet(0xffff);
  //Portc::DirSet(0x0);
    //Portc::DirWrite(0xf);
	 //Portc::DirWrite(0x0);
  //Portc::DirClearAndSet(0x0ff0, 0x0f00);
  //Portc::SetConfiguration(0x0ff0, Portc::Out);
	//Portc::SetConfiguration(0x0ff0, Portc::Out);
	//Porta::SetConfiguration(0x0ff0, Porta::Out);

 // led::SetDirWrite();
//  led::SetConfiguration(led::Port::AltOut);
  
 // led::Set();  
  //Pins::DirSet(0xff);
  //Pins::Write(0x1f);
 // Pins::DirClear(0xff);
  //Pins::DirWrite(0xff);
  
  return 0;
}
