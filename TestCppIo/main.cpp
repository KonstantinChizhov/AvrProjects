#include "ports.h"

void TestOnePortNonConstValue()
{
	uint8_t nonConstValue = Portc::PinRead();
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
	uint8_t nonConstValue = Portc::PinRead();
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
	uint8_t nonConstValue = Portc::PinRead();
 	typedef PinList<Pb1> pins;
	pins::Write(nonConstValue);
}


void OnePinPerPortNonConstValue()
{
	uint8_t nonConstValue = Portc::PinRead();
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
	uint8_t nonConstValue = Portc::PinRead();
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
	uint8_t nonConstValue = Portc::PinRead();
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

void FullPortRndOrderRead()
{
 	typedef PinList<Pa1, Pa0, Pa4, Pa6, Pa3, Pa2, Pa7, Pa5> pins;
	uint8_t value;
	pins::Read(value);
	Portc::Write(value);
}

void FullPortOrderRead()
{
 	typedef PinList<Pa0, Pa1, Pa2, Pa3, Pa4, Pa5, Pa6, Pa7> pins;
	uint8_t value;
	pins::Read(value);
	Portc::Write(value);
}


void RndOrderRead()
{
 	typedef PinList<Pa1, Pa3, Pb2, Pc3, Pd4, Pa5, Pb6, Pc7> pins;
	uint8_t value;
	pins::Read(value);
	Portc::Write(value);
}

int main()
{
	WriteTests();

	while(1)
	{	
	
	}

}


