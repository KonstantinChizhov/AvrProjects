#pragma once
#include <util/delay.h>

template<class DATA, class CLK>
class PdiSoftwarePhisical
{
	public:
	static void Enable()
	{
		DATA::Set();
		_delay_us(120);
		for(uint8_t i = 32; i--;)
		{
			CLK::Togle();
		}
	}

	static void Release()
	{
		DATA::Clear();
	}
	
	static void Write(uint8_t c)
	{
		
	}

	static void Reset()
	{
	
	}
};
