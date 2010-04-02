#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "ports.h"

typedef PinList<Pa0, Pa1, Pa2, Pa3, Pa4, Pa5> ControlPins;
typedef PinList<Pb0, Pb1, Pb2> SersorPins;


template<class ControlPins, class SensorPins, bool HiInverted=false>
class Bldc{
public:
	enum{Ph1Hi = (1 << 0), Ph1Low = (1 << 1), Ph2Hi = (1 << 2), Ph2Low = (1 << 3), Ph3Hi = (1 << 4), Ph3Low = (1 << 5)};
	
	Bldc()
	{
		ControlPins::DirSet(0xff);
		SensorPins::DirClear(0xff);
		_phase = 0;
		_dir = 0;
	}

	void SetDir(uint8_t dir)
	{
		_dir = dir;
	}
		
	void P1()
	{
		if(HiInverted)
			ControlPins::Write(Ph2Low | Ph2Hi | Ph3Hi);
		else
			ControlPins::Write(Ph2Low | Ph1Hi);
	}

	void P2()
	{
		if(HiInverted)
			ControlPins::Write(Ph3Low | Ph1Hi | Ph3Hi);
		else
			ControlPins::Write(Ph3Low | Ph2Hi);
	}

	void P3()
	{
		if(HiInverted)
			ControlPins::Write(Ph1Low | Ph1Hi | Ph2Hi);
		else
			ControlPins::Write(Ph1Low | Ph3Hi);
	}

	void Step()
	{
		switch(_phase)
		{
			case 0: 
				P1();
			break;
			case 1:
				P2();
			break;
			case 2:
				P3();
			break;
		}
		if(_dir)
			_phase++;
		else
			_phase--;
		if(_phase < 0)_phase = 2;
		if(_phase > 2)_phase = 0;
		
	}
	
protected:
	int8_t _phase;
	uint _dir;
};


class Comparator
{
protected:
	enum{AdmuxMask = (1<<MUX0)|(1<<MUX1)|(1<<MUX2)|(1<<MUX3)};
	uint8_t& Control()
	{
		return ACSR;
	}
	uint8_t& Multiplexer()
	{
		return ADMUX;
	}
	void MuxEnable()
	{
		SFIOR |= (1<<ACME);
	}

	void MuxDisable()
	{
		SFIOR &= ~(1<<ACME);
	}

public:
	enum ComparatorChannel {AIN1=255, ADC0=0, ADC1=1, ADC2=2, ADC3=3, ADC4=4, ADC5=6, ADC6=6, ADC7=7, ADC8=8, ADC9=1, ADC10=10};
	enum InterruptMode{Togle=0; Falling=2, Rising=3};

	void SetChannel(ComparatorChannel ch)
	{
		if(ch == AIN1)
		{
			MuxDisable();
		}
		else
		{
			MuxEnable();
			Multiplexer() = Multiplexer() & AdmuxMask | ch;
		}
	}

	void EnableInterrupt(InterruptMode mode)
	{
		Control() |= (1 << ACIE) | (mode << ACIS0);
	}
	
	uint8_t IsSet()
	{
		return Control() & (1<<ACO);
	}
};



ISR(ANA_COMP_vect)
{

}


int main()
{

}
