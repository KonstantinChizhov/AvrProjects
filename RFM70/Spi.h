#include <avr/io.h>
#include <iopins.h>

template
	<
	class SlaveSelectT = IO::Pb4,
	class MosiT	= IO::Pb5,
	class MisoT	= IO::Pb6,
	class ClockT= IO::Pb7
	
	>
class Spi
{
	enum{SPI2X_shift = 2};
	public:

	typedef SlaveSelectT SlaveSelectPin;
	typedef MosiT MosiPin;
	typedef MisoT MisoPin;
	typedef ClockT ClockPin;

	enum ClockDivider
	{
		Div2	= 1 << (SPI2X + SPI2X_shift) | 0 << SPR1 | 0 << SPR0,
		Div4	= 0 << (SPI2X + SPI2X_shift) | 0 << SPR1 | 0 << SPR0,
		Div8	= 1 << (SPI2X + SPI2X_shift) | 0 << SPR1 | 1 << SPR0,
		Div16	= 0 << (SPI2X + SPI2X_shift) | 0 << SPR1 | 1 << SPR0,
		Div32	= 1 << (SPI2X + SPI2X_shift) | 1 << SPR1 | 0 << SPR0,
		Div64	= 0 << (SPI2X + SPI2X_shift) | 1 << SPR1 | 0 << SPR0,
		Div128	= 0 << (SPI2X + SPI2X_shift) | 1 << SPR1 | 1 << SPR0
	};

	enum{ClockDividerMask = 1 << SPR1 | 1 << SPR0};

	static void Init(ClockDivider divider)
	{
		SlaveSelectPin::SetDirWrite();
		MisoPin::SetDirRead();
		MosiPin::SetDirWrite();
		ClockPin::SetDirWrite();
		
		if(divider & (1 << (SPI2X + SPI2X_shift)))
			SPSR |= 1 << SPI2X;
		else 
			SPSR &= ~(1 << SPI2X);
			
		SPCR = (SPCR & ~ClockDividerMask) | 1<<SPE | 1<<MSTR |
				(divider & ClockDividerMask);		

		SlaveSelectPin::Set();
	}

	static uint8_t ReadWrite(uint8_t outValue)
	{
		SPDR = outValue;
		while(!(SPSR & (1<<SPIF)));
		return SPDR;
	}
};


template<class Mosi, class Miso, class Clock>
class SoftSpi
{
	public:
	static uint8_t ReadWrite(uint8_t value)
	{
		Miso::SetDirRead();
		Clock::SetDirWrite();
		Mosi::SetDirWrite();
		
		for(uint8_t i = 0; i < 8;i++)
		{
			Mosi::Set(value & 0x80);
			Clock::Set();
			value <<= 1;
			if(Miso::IsSet())
				value |= 1;
			Clock::Clear();
		}
		return value;
	}
};
