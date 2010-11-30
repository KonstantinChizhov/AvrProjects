#include <avr/io.h>
#include <timer.h>
#include <dispatcher.h>
#include <iopins.h>
#include <avr/interrupt.h>
#include <adc.h>
#include "Servo.h"


using namespace Timers;
using namespace IO;

const unsigned tickFreak = 1000;

typedef Timer0 Timer;
typedef TimerFreqSetup<Timer, tickFreak, 4000000> FSetup;
typedef Dispatcher<4, 4> Disp;



ISR(TIMER0_OVF_vect)
{
	Timer::Set(FSetup::ReloadValue);
	Disp::TimerHandler();
}


int main()
{
	Disp::Init();
	Timer::Start(FSetup::Divider);
	Timer::EnableInterrupt();
	Adc::Init();
	sei();


	while(1)
	{
		Disp::Poll();
	}
}
