#pragma once
#include <util/delay.h>

template<class DATA, class CLK>
class PdiSoftwarePhisical
{
	public:
	static void Enable()
	{
		CLK::SetDirWrite();
		DATA::SetDirWrite();
		DATA::Set();
		_delay_us(120);
		CLK::Set();
		for(uint8_t i = 32; i--;)
		{
			CLK::Togle();
		}
	}

	static void Release()
	{
		DATA::SetDirWrite();
		CLK::SetDirRead();
		DATA::Clear();
	}
	
	static void Write(uint8_t c)
	{
		DATA::SetDirWrite();
		//start bit
		CLK::Clear();
		DATA::Clear();
		CLK::Set();

		// 8 data bits
		uint8_t ones=0;
		for(uint8_t i = 8; i--;)
		{
			CLK::Clear();
			DATA::Set(c&1);
			CLK::Set();
			c >>=1;
			if(c&1)
				ones++;
		}

		//parity bit and 2 stop bits
		ones = ones & 0x07 | 0x06;
		for(uint8_t i = 3; i--;)
		{
			CLK::Clear();
			DATA::Set(ones&1);
			CLK::Set();
			ones >>= 1;
		}
	}

	static void Reset()
	{
	
	}
protected:
	
};
