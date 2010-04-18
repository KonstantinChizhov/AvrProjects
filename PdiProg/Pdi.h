#pragma once
#include <util/delay.h>
#include "util.h"
#include "ProgInterface.h"

template<class DATA, class CLK>
class PdiSoftwarePhisical :public ProgInterface
{
	public:
	void Enable()
	{
		CLK::SetDirWrite();
		DATA::SetDirWrite();
		DATA::Set();
		_delay_us(1);

		SendFrame(0xffff, 16);
		Break();
		Break();
	}

	void Disable()
	{
		DATA::Clear();
		DATA::SetDirRead();
		CLK::SetDirRead();
	}
	
	void Write(uint8_t c)
	{
		DATA::SetDirWrite();
		uint16_t value = c << 2 | 0x1801; //start bit, 8 data bits and 2 stop bits
		if(CountOfOnes(c) & 1)//upend parity bit
			value |= 0x0400;
		
		SendFrame(value, 12);
	}

	uint8_t Read()
	{
		DATA::SetDirRead();
		DATA::Set();//pullup enable
		do
		{
			uint8_t guardCycles=0;
			CLK::Clear();
			if(guardCycles++ < 250) //error
				return 0;
			CLK::Set();	
		}while(DATA::IsSet());

		uint8_t data=0;

		for(uint8_t i = 0; i < 8; i++)
		{
			CLK::Clear();
			data >>= 1;
			CLK::Set();
			if(DATA::IsSet())
				data |= (1<<7);
		}
		//parity and stop bits
		for(uint8_t i = 0; i < 3; )
		{	
			CLK::Clear();
			i++;
			CLK::Set();
		}
		return data;
	}

	void Reset()
	{
		DATA::Clear();
		DATA::SetDirRead();
		CLK::SetDirWrite();
		CLK::Set();
		_delay_ms(10);
		CLK::Clear();
		_delay_us(10);
		CLK::Set();
	}

	void Break()
	{
		DATA::SetDirWrite();
		SendFrame(0xC003, 16);

	}

	void Idle()
	{
		DATA::SetDirWrite();
		SendFrame(0xffff, 12);
	}

protected:
	void SendFrame(uint16_t frame, uint8_t bits)
	{
		for(uint8_t i = 0; i < bits; i++)
		{
			CLK::Clear();
			DATA::Set(frame & 0x01);
			CLK::Set();
			frame >>=1;
		}
	}
};
