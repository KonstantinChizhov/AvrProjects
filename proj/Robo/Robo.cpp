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


ISR(SIG_OUTPUT_COMPARE1A)
{
	Sheduller::TimerHandler();
	Encoder1::CaptureHandler();
}
*/


int main()
{
	DDRB = 0xff;
	//Timer1::Start(Timer1::Div64);
	//Timer1::SetMode(Timer1::Normal);

	//Sheduller::Init();
	
	//Timer0::Start(T0Setup::Divider);
	//Timer0::Set(T0Setup::ReloadValue);

//	

	while(1)
	{
		//EncoderScan();
//		Enc2();
		Encoder1::CaptureHandler();

		PORTB = Encoder1::Value(0)>>8;
		//RTB = EncData;
		//Sheduller::Poll();
	}	
}
