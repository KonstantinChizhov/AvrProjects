#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "iopins.h"
#include "pinlist.h"
#include "dispatcher.h"
#include "HD44780.h"
#include "containers.h"

using namespace IO;

typedef Dispatcher<16, 16> MyDispatcher;

typedef Lcd<PinList<Pa0, Pa1, Pa2, Pa3, Pa4, Pa5, Pa6> > MyLcd;

ISR(TCC0_OVF_vect)
{
	MyDispatcher::TimerHandler();
}

struct Timer0
{
public:
	static void Init()
	{
		TCC0.CTRLA = TC_CLKSEL_DIV1_gc;
		TCC0.INTCTRLA = TC_OVFINTLVL_LO_gc;
	}
};

template<class Lcd,  class TaskMgr>
struct SendToLcd
{
	static void Write(uint8_t value)
	{
		bool wasEmpty = _queue.IsEmpty();
		_queue.Write(value);
		if(wasEmpty)	
			TaskMgr::SetTask(Send);
		
	}

	static void Send()
	{
		uint8_t c;
		if(_queue.Read(c))
			Lcd::Putch(c);
	}
private:
	static Queue<16> _queue;
};

template<class Lcd,  class TaskMgr>
Queue<16> SendToLcd<Lcd, TaskMgr>::_queue;

int main()
{

	Timer0::Init();
	PMIC.CTRL = PMIC_LOLVLEN_bm; 

	sei();

MyLcd::Init();
//	MyDispatcher::SetTask(&MyLcd::Init);
SendToLcd<MyLcd, MyDispatcher>::Write('A');

MyLcd::Puts("123456", 6);
	while(1)
	{
		MyDispatcher::Poll();
	}
}

