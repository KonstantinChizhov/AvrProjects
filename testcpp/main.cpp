
#include "iopins.h"
#include "pinlist.h"
#include <avr/delay.h>

using namespace IO;

typedef Pa1 Led1;
typedef Pa2 Led2;

template <class Led>
void Blink()
{
	Led::Set();
	_delay_ms(100);
	Led::Clear();
	_delay_ms(100);
}

int main()
{
	Led1::SetDirWrite();
	Led2::SetDirWrite();

	while(1)
	{	
		Blink<Led1>();
		Blink<Led2>();
	}
}


