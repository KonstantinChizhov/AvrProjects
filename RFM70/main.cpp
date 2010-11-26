#include <avr/io.h>
#include "Spi.h"
#include "Rfm70.h"
#include <TextFormater.h>
#include <usart.h>

typedef Usart<16, 16> MyUsart;
typedef TextFormater<WaitAdapter<MyUsart> > MyFormater;
typedef Spi<> MySpi;

typedef Rfm70<MySpi, IO::Pb0, IO::Pb1, IO::Pb2> Transiver1;

ISR(USART_RXC_vect)
{
	MyUsart::RxHandler();
}

ISR(USART_UDRE_vect)
{
	MyUsart::TxHandler();
}
__attribute__((OS_main))
int main()
{
	MyUsart::Init(115200);
	MySpi::Init(MySpi::Div32);

	MyFormater formater;
	formater << "Rfm70 programm\r\n";
	Transiver1::Init(MASK_RX_DR | MASK_TX_DS | MASK_MAX_RT | EN_CRC | CRCO | PWR_UP /*| PRIM_RX*/);

	Transiver1::DumpRegs<MyFormater>();

	while(1)
	{
		Transiver1::RfChannel(10);
	}
}
