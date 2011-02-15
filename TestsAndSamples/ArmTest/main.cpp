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

typedef PinList<Pa6, Pa1, Pa4, Pa3, Pa2, Pa5, Pa0> LcdBus;
typedef Lcd<LcdBus> MyLcd;

int main()
{
  Porta::Enable();
  MyLcd::Init();

  MyLcd::Puts("Hello STM32", 13);
	while(1)
	{
		//_delay_ms(100);
		//led::Toggle();
		//led2::Toggle();
	}
  
  return 0;
}
