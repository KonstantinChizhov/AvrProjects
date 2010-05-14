
#include <avr/io.h>

#include "usart.h"
#include "util.h"

#include "Pdi.h"
#include "ports.h"


using namespace IO;


typedef Usart<16, 32> interface;

ISR(USART_UDRE_vect)
{
	interface::TxHandler();
}

ISR(USART_RXC_vect)
{
	interface::RxHandler();
}

typedef Pdi::PdiSoftwarePhisical<Pc0, Pc1> MyPdi;

enum {MsgStart='S', MsgWrite='W', MsgRead='R'};
enum {StateIdle, SateMsgBegin, StateMsgWrite};
enum {Idle, Read, Write};

int main()
{
	MyPdi pdi;
	uint8_t c;
	uint8_t cmd = Idle;

	uint8_t state = StateIdle;

	interface::Init(115200);
	sei();

	IO::Portb::DirWrite(0xff);
	pdi.Enable();

	while(1)
	{
		if(interface::Getch(c))
		{
			switch(state)
			{
				case StateIdle:
					if(c == MsgStart)
						state = SateMsgBegin;
					cmd = Idle;
				break;
				case SateMsgBegin:
					if(c == MsgWrite)
					{
						state = StateMsgWrite;
						cmd = Idle;
					}
					else
					if(c == MsgRead)
					{
						state = StateIdle;
						cmd = Read;
					}
				break;
				case StateMsgWrite:
					state = StateIdle;
					cmd = Write;
				break;
				default:
					state = StateIdle;
					cmd = Idle;
			}
		}
		
		switch(cmd)
		{
			case Read:
				interface::Putch(pdi.ReadByte());
				cmd = Idle;
			break;
			case Write:
				pdi.Write(c);
				interface::Putch(c);
				cmd = Idle;
			break;
			case Idle:
				pdi.Idle();
			break;
		}		
	}
	return 0;
}

extern "C" void __cxa_pure_virtual() { while (1); }
