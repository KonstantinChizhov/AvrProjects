#include <avr/io.h>
#include "Crc16.h"
#include "MkiiProtocol.h"
#include "usart.h"
#include "util.h"
#include "Pdi.h"

using namespace MkII;

typedef WaitAdapter<Usart<16, 16> > interface;
typedef MkIIProtocol<interface> protocol;



int main()
{
	interface::Init(19200);
	
	while(1)
	{
		protocol::PollInterface();
	}
	return 0;
}
