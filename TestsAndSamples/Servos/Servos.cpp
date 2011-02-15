#include <avr/io.h>
#include <timer.h>
#include <dispatcher.h>
#include <iopins.h>
#include <avr/interrupt.h>
#include <adc.h>
//#include "Servo.h"
#include <util/delay.h>

using namespace Timers;
using namespace IO;

//const unsigned tickFreak = 1000;

//typedef Timer0 Timer;
//typedef TimerFreqSetup<Timer, tickFreak, 4000000> FSetup;
//typedef Dispatcher<4, 4> Disp;
typedef Pc7 ServoPin; 

/*
ISR(TIMER0_OVF_vect)
{
	Timer::Set(FSetup::ReloadValue);
	Disp::TimerHandler();
}
*/

void delay(uint16_t val)
{
	_delay_ms(0.7);
	for(uint16_t i = 0; i < val; i++)
		_delay_ms(1.8 / 1023);
}

int main()
{
/*	Disp::Init();
	Timer::Start(FSetup::Divider);
	Timer::EnableInterrupt();
	
	sei();
*/
	ServoPin::SetDirWrite();
	Adc::Init();
	uint16_t adc = Adc::ReadSingle();
	while(1)
	{
		ServoPin::Set();
		delay(adc);
		ServoPin::Clear();
		_delay_ms(7);
		adc = Adc::ReadSingle();
	}
}
