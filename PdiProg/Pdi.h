#pragma once
#include <util/delay.h>
#include "util.h"

template<class DATA, class CLK>
class PdiSoftwarePhisical
{
	public:
	static void Enable()
	{
		CLK::SetDirWrite();
		DATA::SetDirWrite();
		DATA::Set();
		_delay_us(1);

		SendFrame(0xffff, 16);
		Break();
		Break();
	}

	static void Release()
	{
		DATA::Clear();
		DATA::SetDirRead();
		CLK::SetDirRead();
	}
	
	static void Write(uint8_t c)
	{
		DATA::SetDirWrite();
		uint16_t value = c << 2 | 0x1801; //start bit, 8 data bits and 2 stop bits
		if(CountOfOnes(c) & 1)//upend parity bit
			value |= 0x0400;
		
		SendFrame(value, 12);
	}

	static uint8_t Read()
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

	static void Reset()
	{
	
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
