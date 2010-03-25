#ifndef USART_HPP
#define USART_HPP

#include <avr/io.h>
#include "containers.h"
#include <avr/interrupt.h>

struct UsartTraits
{
	uint8_t Read()
	{
		return _UDR_;
	}

	void Write(uint8_t value)
	{
		_UDR_ = value;
	}

	void SetBaundRate(uint32_t baund)
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
			_UCSRA_ = (1 << U2X);
			ubrrToUse = ubrr2x;
		}
		else
		{
			_UCSRA_ = 0x00;
			ubrrToUse = ubrr;
		}
		_UBRRL_=(ubrrToUse);
		_UBRRH_=(ubrrToUse)>>8;
	}

	void EnableTxRx()
	{
		_UCSRB_ = 0x00; 
		_UCSRC_ = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);
		_UCSRB_ = (1 << RXCIE) | (0 << TXCIE) | (1 << UDRIE) | (1 << RXEN) | (1 << TXEN);
	}

	bool CanWriteData()
	{
		return _UCSRA_ & (1<<UDRE);
	}

};

template<int TxSize, int RxSize, class Traits>
class Usart
{
public:
	static void Init(unsigned long baund)
	{
		Traits::SetBaundRate();
		Traits::EnableTxRx();
	}

	static uint8_t Putch(uint8_t c)__attribute__ ((noinline))
	{
		if(_tx.IsEmpty())
		{
			while(!Traits::CanWriteData());
			Traits::Write(c);
			return 1;
		}else 
		return _tx.Write(c);
	}

	static uint8_t Getch(uint8_t &c)__attribute__ ((noinline))
	{
		return _rx.Read(c);
	}

	inline void USART_UDRE_Handler()
	{
		uint8_t c;
		if(Usart::_tx.Read(c))
			Traits::Write(c);
	}

	inline void USART_RXC_Handler()
	{
		if(!Usart::_rx.Write(Traits::Read()))//buffer overlow
		{
			//TODO: error handling
			Usart::_rx.Clear();
		}	
	}

protected:
	static Queue<RxSize> _rx;
	static Queue<TxSize> _tx;
};

template<class Traits, int TxSize, int RxSize>
Queue<RxSize> Usart<Traits, TxSize, RxSize>::_rx;
template<class Traits, int TxSize, int RxSize>
Queue<TxSize> Usart<Traits, TxSize, RxSize>::_tx;


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
