#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "ports.h"
#include "comparator.h"
#include "timer.h"

typedef PinList<Pa0, Pa1, Pa2, Pb0, Pb1, Pb2> ControlPins;
typedef PinList<Pa4, Pa5, Pa7> SersorPins;
typedef Pa3 boostPin;

template<class ControlPins, class SensorPins, bool HiInverted=false>
class Bldc{
public:
	enum{
		
		Ph1Low = 	(1 << 0),
		Ph2Low = 	(1 << 1),
		Ph3Low = 	(1 << 2),
		Ph1Hi = 	(1 << 3), 
		Ph2Hi = 	(1 << 4), 
		Ph3Hi = 	(1 << 5)
	};
	
	static void Init()
	{
		ControlPins::DdrSet(0xff);
		SensorPins::DdrClear(0xff);
		_phase = 0;
		_dir = 0;
	}

	static inline void SetDir(uint8_t dir)
	{
		_dir = dir;
	}
		
	static inline void P1()
	{
		//if(HiInverted)
			ControlPins::Write(0b00011001);
	//	else
		//	ControlPins::Write((uint8_t)(Ph2Low | Ph1Hi));
	}

	static inline void P2()
	{
	//	if(HiInverted)
			ControlPins::Write(0b00110010);
	//else
		//	ControlPins::Write((uint8_t)(Ph3Low | Ph2Hi));
	}

	static inline void P3()
	{
		//if(HiInverted)
			ControlPins::Write(0b00101100);
	//	else
		//	ControlPins::Write((uint8_t)(Ph1Low | Ph3Hi));
	}

	static inline void Step()
	{
		switch(_phase)
		{
			case 0: 
				P1();
				Comparator::SetChannel(Comparator::ADC4);
			break;
			case 1:
				P2();
				Comparator::SetChannel(Comparator::ADC3);
			break;
			case 2:
				P3();
				Comparator::SetChannel(Comparator::AIN1);
			break;
		}
		//if(_dir)
			_phase++;
		//else
		//	_phase--;
		//if(_phase < 0)_phase = 2;
		if(_phase > 2)_phase = 0;
		
	}
	
protected:
	static int8_t _phase;
	static uint8_t _dir;
};

template<class ControlPins, class SensorPins, bool HiInverted>
int8_t Bldc<ControlPins, SensorPins, HiInverted>::_phase;
template<class ControlPins, class SensorPins, bool HiInverted>
uint8_t Bldc<ControlPins, SensorPins, HiInverted>::_dir;

typedef Bldc<ControlPins, SersorPins, true> motor;

ISR(TIMER0_OVF0_vect)
{
	motor::Step();
}


ISR(ANA_COMP_vect)
{
	Comparator::DisableInterrupt();
	Timer0::Reset();
	motor::Step();
	_delay_us(500);
	Comparator::EnableInterrupt(Comparator::Rising);
}


int main()
{
	Comparator::SetChannel(Comparator::ADC4);
	Comparator::EnableInterrupt(Comparator::Rising);
	boostPin::SetDirWrite();
	motor::Init();
	Timer0::Start(Timer0::Ck1024);
	Timer0::EnableOverflowInterrupt();
	sei();
	while(1)
	{
		boostPin::Togle();
		_delay_us(100);
	}
}
