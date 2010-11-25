#include <avr/io.h>
#include "Spi.h"
#include "Rfm70.h"
int main()
{
	while(1)
	{
		PORTB = Bank1Regs[0];
	}
}
