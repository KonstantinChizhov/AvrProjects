#ifndef USART_HPP
#define USART_HPP

#include <avr/io.h>
#include "util.h"
#include <avr/interrupt.h>

#ifndef RX_BUFFER_SIZE
#define RX_BUFFER_SIZE 8
#endif

#ifndef TX_BUFFER_SIZE
#define TX_BUFFER_SIZE 8
#endif

class Usart
{
public:
	static void Init(unsigned long baund)
	{
		unsigned int ubrr = (F_CPU/16/baund-1);
		unsigned int ubrr2x 	(F_CPU/8/baund-1);
		unsigned long rbaund = (F_CPU/16/(ubrr+1));	
 		unsigned long rbaund2x (F_CPU/8/(ubrr2x+1));

		unsigned long err1;
		if(baund > rbaund)
			err1 = (baund - rbaund)*1000/baund;
		else
			err1 = (rbaund - baund)*1000/rbaund;

		unsigned long err2;
		if(baund > rbaund2x)
			err2 = (baund - rbaund2x)*1000/baund;
		else
			err2 = (rbaund2x - baund)*1000/rbaund2x;
		
		unsigned char ucsra;
		unsigned int ubrrToUse;
		if(err1 > err2)
		{
			ucsra = (1 << U2X);
			ubrrToUse = ubrr2x;
		}
		else
		{
			ucsra = 0x00;
			ubrrToUse = ubrr;
		}
		UCSRB = 0x00; 
		UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);
		UCSRB = (1 << RXCIE) | (1 << TXCIE) | (1 << UDRIE) | (1 << RXEN) | (1 << TXEN);
		UBRRL=(ubrrToUse);
		UBRRH=(ubrrToUse)>>8;
		UCSRA = ucsra;
	}

	static uint8_t Putch(uint8_t c)__attribute__ ((noinline))
	{
		if(_tx.IsEmpty())
		{
			while( !( UCSRA & (1<<UDRE)) );
			UDR = c;
			return 1;
		}else 
		return _tx.Write(c);
	}

	static uint8_t Getch(uint8_t &c)__attribute__ ((noinline))
	{
		return _rx.Read(c);
	}

	template<class T>
	static void Write(T value)
	{
		Write(&value, sizeof(T));
	}

	inline static void USART_UDRE_Handler();
	inline static void USART_RXC_Handler();
protected:
	static Queue<RX_BUFFER_SIZE> _rx;
	static Queue<TX_BUFFER_SIZE> _tx;
};

Queue<RX_BUFFER_SIZE> Usart::_rx;
Queue<TX_BUFFER_SIZE> Usart::_tx;

inline void Usart::USART_UDRE_Handler()
{
	uint8_t c;
	if(Usart::_tx.Read(c))
		UDR = c;	
}

inline void Usart::USART_RXC_Handler()
{
	if(!Usart::_rx.Write(UDR))//buffer overlow
	{
		//TODO: error handling
		Usart::_rx.Clear();
	}	
}

EMPTY_INTERRUPT(USART_TXC_vect)

ISR(USART_UDRE_vect)
{	
	Usart::USART_UDRE_Handler();
}
 

ISR(USART_RXC_vect)
{
	Usart::USART_RXC_Handler();
}

#endif
