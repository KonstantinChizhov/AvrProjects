#include "main.h"

using namespace Timers;

ISR(SIG_INTERRUPT0)
{
	Ir::CaptureHandler();
}

ISR(SIG_OVERFLOW0)
{
	Ir::TimeoutHandler();
}

ISR(SIG_OUTPUT_COMPARE1A)
{
	Sheduller::TimerHandler();
}

int main()
{
	Timer1::Start(Timer1::Div64);
	Timer1::SetMode(Timer1::CtcToOcr1a);
	Sheduller::Init();

	while(1)
	{
		Sheduller::Poll();
	}	
}
