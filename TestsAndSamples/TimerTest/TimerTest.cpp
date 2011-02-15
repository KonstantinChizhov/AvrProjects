#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "timer.h"

using namespace Timers;

ISR(SIG_OVERFLOW0)
{

}

int main()
{
	Timer0::Start(Timer0::Div1);
	Timer0::EnableInterrupt();
//	Timer0::OutputCompare<0>::Set(100);
//	Timer0::OutputCompare<0>::EnableInterrupt();

	return 0;
}
