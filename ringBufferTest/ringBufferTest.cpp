#include <avr/io.h>
#include "containers.h"
#include <avr/interrupt.h>
Queue<16> buffer;

ISR(USART_RXC_vect)
{
		if(!buffer.Write(UDR))
		{
			//buffer overlow
			buffer.Clear();
		}
}

int main()
{
	uint8_t c;
	if(buffer.Read(c))
		PORTC = c;
}
