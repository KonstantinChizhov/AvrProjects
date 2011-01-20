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
typedef IO::Pc9 led2;

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

typedef PinList<Pa0, Pa1, Pa2, Pa3, Pa4, Pa5, Pa6> LcdBus;
typedef Lcd<LcdBus> MyLcd;

int main()
{
  RCC_APB2ENR |= 1 << 2 | 1 << 3 | 1 << 4;
 // MyLcd::Init();
  //LcdBus::SetConfiguration<LcdBus::Out, 0xff>();
 // LcdBus::Pin<2>::Set();
  //Porta::Set(PinList<LcdBus::Pin<0>, LcdBus::Pin<1> >::Length);
	PinList<LcdBus::Pin<0>, LcdBus::Pin<1> >::Set<0xff>();
	PinList<Pa0, Pa1>::Set<0xff>();
 // led::SetConfiguration<led::Port::Out>();
 // led2::SetConfiguration(led2::Port::Out);
 // led2::SetConfiguration(led2::Port::In);
 // led2::SetConfiguration(led2::Port::Out);
 // led2::SetConfiguration(led2::Port::In);
 // led::Set();
	while(1)
	{
		_delay_ms(100);
		led::Toggle();
		led2::Toggle();
	}
  
  return 0;
}
