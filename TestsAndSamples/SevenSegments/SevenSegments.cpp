#include <avr/io.h>
#include <iopins.h>
#include <pinlist.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

using namespace IO;

#include <LedDisplay.h>
typedef LedDisplay<
	PinList<Pb0Inv, Pb1Inv, Pb2Inv, Pb3Inv, Pb4Inv, Pb5Inv, Pb6Inv, Pb7Inv>,
	PinList<Pc1Inv, Pc2Inv, Pc3Inv, Pc4Inv>
	//PinList<Pc1, Pc2, Pc3, Pc4>
	> Disp;

int main()
{
	Disp::WriteDec(12345);
	//Disp::Write(-123);
	while(1)
	{
		_delay_ms(5);
		Disp::Update();
	}
}
