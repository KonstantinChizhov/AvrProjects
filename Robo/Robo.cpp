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
	Timer1::SetMode(Timer1::Normal);
	Timer0::Start(Timer0::Div8);
	Timer0::SetMode(Timer0::PwmPhaseCorrect);

	Sheduller::Init();

	while(1)
	{
		Sheduller::Poll();
	}	
}
