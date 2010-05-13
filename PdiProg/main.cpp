#include <avr/io.h>

#include "usart.h"
#include "util.h"

#include "Crc16.h"
#include "MkiiProtocol.h"
#include "Pdi.h"
#include "ports.h"

//#include "UsbFifo.h"

using namespace MkII;
using namespace IO;


typedef Usart<16, 32> interface;

typedef MkIIProtocol
	<
		interface, 
		Pdi::PdiSoftwarePhisical<Pd5, Pd6>
	> Protocol;


ISR(USART_UDRE_vect)
{
	interface::TxHandler();
}

ISR(USART_RXC_vect)
{
	interface::RxHandler();
}

Protocol protocol;

__attribute__ ((OS_main))
int main()
{
	interface::Init(19200);
	sei();

	IO::Portb::DirWrite(0xff);

	while(1)
	{
		protocol.PollInterface();
	}
	return 0;
}

extern "C" void __cxa_pure_virtual() { while (1); }
