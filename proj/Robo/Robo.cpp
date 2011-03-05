#include "main.h"

using namespace Timers;

/*
ISR(SIG_INTERRUPT0)
{
	Ir::CaptureHandler();
}

ISR(SIG_OVERFLOW0)
{
	Ir::TimeoutHandler();
}


ISR(SIG_OVERFLOW0)
{
	Timer0::Set(T0Setup::ReloadValue);
	Sheduller::TimerHandler();
}

*/

int main()
{
	//Timer1::Start(Timer1::Div64);
	//Timer1::SetMode(Timer1::Normal);
//
	//Sheduller::Init();
	
//	Timer0::Start(T0Setup::Divider);
//	Timer0::Set(T0Setup::ReloadValue);



	while(1)
	{
	Util::delay_us<10000000, F_CPU>();

	//	Sheduller::Poll();
	}	
}
