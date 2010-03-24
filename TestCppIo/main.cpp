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


int main()
{
	TestOnePortNonConstValue();
	TestOnePortConstValue();
	TestThreePortsNonConstValue();
	TestThreePortsConstValue();

	while(1)
	{	
	
	}

}


