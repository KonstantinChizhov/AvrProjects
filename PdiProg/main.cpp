#include <avr/io.h>
#include "Crc16.h"
#include "MkiiProtocol.h"
#include "usart.h"
#include "util.h"
#include "Pdi.h"
#include "ports.h"

using namespace MkII;
using namespace IO;

typedef WaitAdapter<Usart<16, 32> > interface;
typedef MkIIProtocol
	<
		interface, 
		PdiSoftwarePhisical<Pd5, Pd6>
		> protocol;

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
	IO::Portb::DirWrite(0xff);
	protocol::Init();
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

extern "C" void __cxa_pure_virtual() { while (1); }
