#include "main.h"
#include <usart.h>
#include <stdint.h>
#include <avr/pgmspace.h>
#include <TextFormater.h>

typedef Usart<8, 8> usart;
TextFormater<WaitAdapter<usart>, 16 > debug;

using namespace Timers;

ISR(USART_RXC_vect)
{
	usart::RxHandler();
}

ISR(USART_UDRE_vect)
{
	usart::TxHandler();
}

/*
ISR(SIG_INTERRUPT0)
{
	Ir::CaptureHandler();
}

ISR(SIG_OVERFLOW0)
{
	Ir::TimeoutHandler();
}
*/

ISR(SIG_OVERFLOW0)
{
	Timer0::Set(T0Setup::ReloadValue);
	Sheduller::TimerHandler();
}

void Start()
{
	LeftMotor::Forward();
	RightMotor::Forward();
	Sheduller::SetTimer(LeftMotor::Break, 2000);
	Sheduller::SetTimer(RightMotor::Break, 2000);
	Sheduller::SetTimer(Start, 4000);
}


uint16_t keys;
uint8_t buffer[30];


__attribute__((OS_main))
int main()
{
	//Timer1::Start(Timer1::Div64);
	//Timer1::SetMode(Timer1::Normal);
//


	usart::Init(115200/2);
	debug << "Hello!!!";

	Sheduller::Init();

	Timer0::Start(T0Setup::Divider);
	Timer0::Set(T0Setup::ReloadValue);
	Timer0::EnableInterrupt();

	Transiver::Init();

	Transiver::SwitchToRxMode();

	Transiver::SetRxAddress<0>(0x12345678, 0x90);
	//Start();
	sei();
Transiver::DumpRegs(debug);
	while(1)
	{
		if(Transiver::Recive(&keys))
		{
			LeftMotor::Forward();
			RightMotor::Forward();
			Sheduller::SetTimer(LeftMotor::Break, 100);
			Sheduller::SetTimer(RightMotor::Break, 100);
		}
		Sheduller::Poll();
		//debug << ".";
		Util::delay_ms<1, F_CPU>();
	}
}
