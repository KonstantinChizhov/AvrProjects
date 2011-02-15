
#include <avr/io.h>
#include<avr/interrupt.h>
#include <iopins.h>
#include <util/delay.h>
#include <usart.h>
#include <TextFormater.h>

typedef Usart<16, 16> usart;
TextFormater<usart, 10> debug;

using namespace IO;

ISR(USART_UDRE_vect)
{
	usart::TxHandler();
}

ISR(USART_RXC_vect)
{
	usart::RxHandler();
}

template<class Line>
class OneWire
{
//	void DelayUs(uint16_t us)
	//{
	//	const uint16_t cycles = F_CPU
	//}
	static uint8_t IoBit(uint8_t value)
	{
		Line::SetDirWrite();
		_delay_us(2);
		if(value)
		{
			Line::SetDirRead();
		}
		_delay_us(13);
		if(!Line::IsSet())
		{
			value = 0;
		}
		_delay_us(60-15-2);
		Line::SetDirRead();
		_delay_us(1);
		return value;
	}

	public:
	static bool Reset()
	{
		Line::Clear();
		Line::SetDirWrite();
		_delay_us(480);
		Line::SetDirRead();
		_delay_us(10);
		if(Line::IsSet())
		{
			_delay_us(50);
			if(!Line::IsSet())
				return true;
		}
		return false;
	}

	static void Write(uint8_t value)
	{
		for(uint8_t i = 0; i<8; i++)
		{
			IoBit(value&1);
			value >>= 1;
		}
	}

	static uint8_t Read()
	{
		uint8_t value = 0;
		for(uint8_t i = 0; i<8; i++)
		{
			value >>= 1;
			value |= IoBit(0x80);
		}
		return value;
	}

	static uint64_t ReadRom()
	{
		union{
			uint8_t bytes[8];
			uint64_t rom;
		};

		Write(0x33);
		bytes[0] = Read();
		bytes[1] = Read();
		bytes[2] = Read();
		bytes[3] = Read();
		bytes[4] = Read();
		bytes[5] = Read();
		bytes[6] = Read();
		bytes[7] = Read();

		return rom;
	}

	static uint64_t SearchRom(uint64_t prev)
	{
		return 0;
	}
	
	static void ConvertT()
	{
		debug << "Converting:";
		Write(0x44);
		int v;
		do
		{
			v = Read();
			debug << ".";
			_delay_ms(10);
		}
		while( v == 0 );
		debug << "\n\r";
	}

	static int16_t ReadTemp()
	{
		Write(0xbe);
		union
		{
			int16_t temp;
			uint8_t bytes[2];
		};
		bytes[0] = Read();
		bytes[1] = Read();

		return temp;
	}
};

typedef OneWire<Pc0> Wire;
typedef Pb0 Led;

int main()
{
	usart::Init(115200);
	
	sei();
	debug << "Hello\n\r";

	Led::SetDirWrite();
	while(1)
	{
		Led::Set(Wire::Reset());
		_delay_ms(1);
		Wire::ReadRom();
		_delay_ms(1);
		Wire::ConvertT();
	_delay_ms(1);
		Led::Set(Wire::Reset());
		_delay_ms(1);
		Wire::ReadRom();
		debug << "T =\t" << Wire::ReadTemp()*0.0625f << "\n\r";
_delay_ms(100);
	}
}
