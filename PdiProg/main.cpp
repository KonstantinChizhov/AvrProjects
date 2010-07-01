#include <avr/io.h>

#include "usart.h"
#include "util.h"

#include "Crc16.h"
#include "MkiiProtocol.h"
#include "Pdi.h"
#include "ioports.h"
#include "iopins.h"
//#include "UsbFifo.h"

using namespace MkII;
using namespace IO;


typedef Usart<16, 32> CommInterface;
typedef Pdi::PdiSoftwarePhisical<Pc1, Pc0> PdiInterface;
typedef MkIIProtocol
	<
		CommInterface, 
		PdiInterface
	> Protocol;


ISR(USART_UDRE_vect)
{
	CommInterface::TxHandler();
}

ISR(USART_RXC_vect)
{
	CommInterface::RxHandler();
}

ISR(TIMER0_OVF_vect)
{
	PdiInterface::TimerHandler();
}

Protocol protocol;

__attribute__ ((OS_main))
int main()
{
	CommInterface::Init(19200);
	sei();

	IO::Portb::DirWrite(0xff);

	while(1)
	{
		protocol.PollInterface();
	}
	return 0;
}

extern "C" void __cxa_pure_virtual() { while (1); }
