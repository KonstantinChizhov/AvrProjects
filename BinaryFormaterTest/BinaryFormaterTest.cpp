#include <usart.h>
#include <util.h>
#include <ports.h>

typedef BinaryFormater< WaitAdapter < Usart<16, 32> > > usart;

ISR(USART_UDRE_vect)
{
	usart::TxHandler();
}

ISR(USART_RXC_vect)
{
	usart::RxHandler();
}

int main()
{
	usart::Init(115200);

	uint32_t v ;
	v = usart::Read<uint32_t>();
	
	usart::Write(v);
	while(1)
	{
		
	}
}
