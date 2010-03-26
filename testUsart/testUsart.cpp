#include "Usart.h"

Usart<512, 512> usart;


ISR(USART_UDRE_vect)
{	
	usart.TxHandler();
}


ISR(USART_RX_vect)
{
	usart.RxHandler();
}


int main()
{
	usart.Init(115200);
	while(1)
	{
	
	}
}
