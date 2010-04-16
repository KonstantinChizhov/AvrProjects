#include <avr/io.h>
#include "Crc16.h"
#include "MkiiProtocol.h"
#include "usart.h"
#include "util.h"
#include "Pdi.h"

using namespace MkII;

typedef WaitAdapter<Usart<16, 32> > interface;
typedef MkIIProtocol<interface> protocol;

ISR(USART_UDRE_vect)
{
	interface::TxHandler();
}

ISR(USART_RXC_vect)
{
	interface::RxHandler();
}

int main()
{
	interface::Init(19200);
	sei();
	while(1)
	{
		protocol::PollInterface();
		//interface::Getch(c);
		//interface::Putch(c);
	}
	return 0;
}
