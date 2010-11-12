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
	Encoder1::CaptureHandler();
}


int main()
{

	Timer1::Start(Timer1::Div64);
	Timer1::SetMode(Timer1::Normal);

	Sheduller::Init();
	
	typedef TimerFreqSetup<Timer0, 1000> T0Setup;
	Timer0::Start(T0Setup::Divider);
	Timer0::Set(T0Setup::ReloadValue);

	while(1)
	{
		Sheduller::Poll();
	}	
}
