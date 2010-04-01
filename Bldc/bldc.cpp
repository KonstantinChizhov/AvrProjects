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



int main()
{

}
