#include "ports.h"

void TestOnePortNonConstValue()
{
	uint8_t nonConstValue = PORTC;
 	typedef PinList<Pa0, Pa1, Pa2, Pa3> pins;
	pins::Write(nonConstValue);
}

void TestOnePortConstValue()
{
 	typedef PinList<Pa0, Pa1, Pa2, Pa3> pins;
	pins::Write(0x55);
}

void TestThreePortsNonConstValue()
{
	uint8_t nonConstValue = PORTC;
 	typedef PinList<Pa0, Pb1, Pc2, Pa3, Pb3, Pc3> pins;
	pins::Write(nonConstValue);
}

void TestThreePortsConstValue()
{
 	typedef PinList<Pa0, Pb1, Pc2, Pa3, Pb3, Pc3> pins;
	pins::Write(0x55);
}

void OnePinTest()
{
 	typedef PinList<Pb1> pins;
	pins::Write(0x1);
}

void OnePinNonConstValueTest()
{
	uint8_t nonConstValue = PORTC;
 	typedef PinList<Pb1> pins;
	pins::Write(nonConstValue);
}


void OnePinPerPortNonConstValue()
{
	uint8_t nonConstValue = PORTC;
 	typedef PinList<Pa0, Pb1, Pc2> pins;
	pins::Write(nonConstValue);
}

void OnePinPerPortConstValue()
{
 	typedef PinList<Pa0, Pb1, Pc2> pins;
	pins::Write(0x55);
}

void FullPortConstValue()
{
 	typedef PinList<Pa0, Pa1, Pa2, Pa3, Pa4, Pa5, Pa6, Pa7> pins;
	pins::Write(0x55);
}

void FullPortNonConstValue()
{
	uint8_t nonConstValue = PORTC;
 	typedef PinList<Pa0, Pa1, Pa2, Pa3, Pa4, Pa5, Pa6, Pa7> pins;
	pins::Write(nonConstValue);
}

void FullPortConstValueRndOrder()
{
 	typedef PinList<Pa1, Pa0, Pa4, Pa6, Pa3, Pa2, Pa7, Pa5> pins;
	pins::Write(0x55);
}

void FullPortNonConstValueRndOrder()
{
	uint8_t nonConstValue = PORTC;
 	typedef PinList<Pa1, Pa0, Pa4, Pa6, Pa3, Pa2, Pa7, Pa5> pins;
	pins::Write(nonConstValue);
}

void WriteTests()
{
	TestOnePortNonConstValue();
	TestOnePortConstValue();
	TestThreePortsNonConstValue();
	TestThreePortsConstValue();
	OnePinTest();
	OnePinNonConstValueTest();
	OnePinPerPortNonConstValue();
	OnePinPerPortConstValue();
	FullPortConstValue();
	FullPortConstValueRndOrder();
}

int main()
{
	WriteTests();

	while(1)
	{	
	
	}

}


