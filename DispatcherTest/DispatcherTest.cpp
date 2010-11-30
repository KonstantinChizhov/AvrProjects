#include <avr/io.h>
#include <timer.h>
#include <dispatcher.h>
#include <iopins.h>
#include <avr/interrupt.h>
#include <adc.h>

using namespace Timers;
using namespace IO;

const unsigned tickFreak = 1000;

typedef Timer0 Timer;
typedef TimerFreqSetup<Timer, tickFreak, 4000000> FSetup;
typedef Dispatcher<4, 4> Disp;
typedef Pc0 TestPin;

ISR(TIMER0_OVF_vect)
{
	Timer::Set(FSetup::ReloadValue);
	Disp::TimerHandler();
}

void Pause();

void Pulse()
{
	TestPin::Set();
	Disp::SetTimer(Pause, 10);
}

void Pause()
{
	TestPin::Clear();
	Disp::SetTimer(Pulse, 100);
}

int main()
{
	Disp::Init();
	Timer::Start(FSetup::Divider);
	Timer::EnableInterrupt();
	TestPin::SetDirWrite();
	Adc::Init();
	sei();


	Pulse();

	while(1)
	{
		Disp::Poll();
	}
}
