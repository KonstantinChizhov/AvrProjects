
#if defined(__ICCAVR__) || defined(__AVR__)
#include "AVR/spi.h"
#endif

#include <iopins.h>

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
