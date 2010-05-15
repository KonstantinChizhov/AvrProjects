
#include <avr/io.h>

#include "usart.h"
#include "util.h"

#include "Pdi.h"
#include "ports.h"
#include "timer.h"

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

Pdi::PdiSoftwarePhisical<Pc1, Pc0> pdi;

ISR(TIMER0_OVF_vect)
{
	pdi.TimerHandler();
}

int main()
{
	sei();
	pdi.Enable();
	interface::Init(115200);
	IO::Portb::DirWrite(0xff);

	while(1)
	{
		pdi.WriteByte(Pdi::CMD_STCS | Pdi::CTRL_REG);
		pdi.WriteByte(0x3);

		pdi.WriteByte(Pdi::CMD_LDCS | Pdi::RESET_REG);
		uint8_t c = pdi.ReadByte();
		IO::Portb::Write(c);
		interface::Putch(c);

		pdi.Break();
		pdi.WriteByte(Pdi::CMD_LDCS | Pdi::CTRL_REG);
		c = pdi.ReadByte();
		IO::Portb::Write(c);
		interface::Putch(c);

		pdi.Break();
		pdi.WriteByte(Pdi::CMD_LDCS | Pdi::STATUS_REG);
		c = pdi.ReadByte();
		IO::Portb::Write(c);
		interface::Putch(c);

		pdi.Break();
		_delay_ms(10);
	}
	return 0;
}

extern "C" void __cxa_pure_virtual() { while (1); }
