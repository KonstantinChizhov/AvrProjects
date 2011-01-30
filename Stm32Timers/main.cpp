

#define USE_PORTA
#define USE_PORTC
#define USE_PORTB
#define USE_TIM1
#include <iopins.h>
#include <pinlist.h>
#include <spi.h>
#include <latch.h>
#include <timers.h>

using namespace IO;
using namespace Timers;

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

int main()
{
  //Porta::Enable();
  Portc::Enable();
  led::SetConfiguration(led::Port::Out);
  led2::SetConfiguration(led2::Port::Out);
  
  led::Set();
  
 Timer1::Start(Timer1::Div1);
	while(1)
	{
		//_delay_ms(1000);
		led::Set(Timer1::Get() & 0x0400);
		led2::Toggle();
	}
}
