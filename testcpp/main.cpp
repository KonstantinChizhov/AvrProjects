#include "dispatcher.h"
#include "ports.h"
#include "HD44780.h"

enum{ TIMER_INTERVAL = 187};
enum{ TIMER_START = (256-TIMER_INTERVAL)};

//typedef Dispatcher<16, 10> dispatcher;
TextFormater<Lcd<Pa0, Pa1, Pa2, PinList<Pa3, Pa4, Pa5, Pa6> > > lcd;

/*
ISR(TIMER0_OVF_vect)
{
	TCNT0 = TIMER_START;
	dispatcher::TimerHandler();
}

void Timer0Init()
{
	TCNT0 = TIMER_START;
	TCCR0 = (0<<CS02) | (1<<CS01) | (1<<CS00);
	TIMSK |= (1<<TOIE0);
}

void F2();

void F1()
{
	dispatcher::SetTimer(F2, 100);
	Pa0::Set();
}

void F2()
{
	dispatcher::SetTimer(F1, 100);
	Pa0::Clear();
}

*/
int main()
{
	//dispatcher::Init();
	lcd.Init();
	//Timer0Init();
	//Pa0::SetDirWrite();
	//sei();
	//F1();
	lcd << "123456789";
	lcd << 123;
	lcd << 444;
	lcd << 555;
	lcd << 777;
	lcd << "abc";
	while(1)
	{	
		//dispatcher::Poll();
	}

}


