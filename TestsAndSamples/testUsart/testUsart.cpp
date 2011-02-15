#include "Usart.h"

typedef Usart<16, 16> usart;


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
	uint8_t c;
	while(1)
	{	
		if(usart::Getch(c))
			usart::Putch(c);
	}
}
