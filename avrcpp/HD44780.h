#ifndef HD44780_HPP
#define HD44780_HPP

#include "ports.h"
#include <util/delay.h>
#include "static_assert.h"

class LcdBase
{
	protected:
	LcdBase(){}
	static void Delay()__attribute__ ((noinline))
	{
		_delay_us(200); 
	}
};

template<class D4, class D5, class D6, class D7>
class PinsLcdDataBus
{

protected:
	PinsLcdDataBus(){}

	static void SetDataRead()
	{
		D4::SetDirRead();
		D5::SetDirRead();
		D6::SetDirRead();
		D7::SetDirRead();
	}

	static void SetDataWrite()
	{
		D4::SetDirWrite();
		D5::SetDirWrite();
		D6::SetDirWrite();
		D7::SetDirWrite();
	}

	static void Write4(uint8_t c) //4 msb
	{
		D4::Set(c&16);
		D5::Set(c&32);
		D6::Set(c&64);
		D7::Set(c&128);
	}

	static uint8_t Read4()
	{
		uint8_t res = 0;
		if(D4::IsSet())
			res|=(1);
		if(D5::IsSet())
			res|=(1<<1);
		if(D6::IsSet())
			res|=(1<<2);
		if(D7::IsSet())
			res|=(1<<3);
		return res;
	}
};



template<class PORT, uint8_t DATA_BUS_OFFSET>
class PortLcdDataBus
{

protected:
	
	BOOST_STATIC_ASSERT(DATA_BUS_OFFSET < 5);

	enum{PortMask = 0x0f << DATA_BUS_OFFSET};

	PortLcdDataBus(){}

	static void SetDataRead()
	{
		PORT::dir() &= (uint8_t)~PortMask;
	}

	static void SetDataWrite()
	{
		PORT::dir() |= (uint8_t)PortMask;
	}

	static void Write4(uint8_t c) //4 msb
	{
		PORT::data() = (PORT::data() & (uint8_t)~PortMask) | (c >> (4-DATA_BUS_OFFSET));
	}

	static uint8_t Read4()
	{
		return (PORT::pin() & PortMask) >> DATA_BUS_OFFSET;
	}
};


template<class RS, class RW, class E, class DATA_BUS, uint8_t LINE_WIDTH=8, uint8_t LINES=2>
class Lcd: public LcdBase, DATA_BUS
{
public:
	static uint8_t LineWidth()
	{
		return LINE_WIDTH;
	}

	static uint8_t Lines()
	{
		return LINES;
	}

	static void Init()
	{
		E::SetDirWrite();
		RW::SetDirWrite();
		RS::SetDirWrite();
		DATA_BUS::SetDataWrite();
		RW::Clear();
		RS::Clear();
		DATA_BUS::Write4(0x30); 
		Strobe();
		Strobe();
		Strobe();
		_delay_ms(60);
		DATA_BUS::Write4(0x20); // set 4 bit mode 
		Strobe();
		Write(0x28); // 4 bit mode, 1/16 duty, 5x8 font 
	
		Write(0x08); // display off 
		Write(0x0E); // display on, blink curson on 
		Write(0x06); // entry mode 
	}

	Lcd()
	{
		Init();
	}

	static void Clear(void)
	{
		while(Busy());
		RS::Clear();
		Write(0x01); 
		while(Busy());
		Write(0x02);
	}

	static void Goto(uint8_t pos)
	{
		while(Busy());
		RS::Clear();
		Write(0x80+pos);
	}

	static void Goto(uint8_t x, uint8_t y)
	{
		while(Busy());
		RS::Clear();
		if(y == 1)
			x += 0x40;
		Write(0x80+x);
	}

	static void Home(void)
	{
		while(Busy());
		RS::Clear();
		Write(0x02);
	}

	static void Puts(const char *s, uint8_t len)
	{
		while(Busy());
		RS::Set(); // write characters 
		while(len-- && *s){
			Write(*s++);
		} 
	}

	static void Putch(char c)
	{
		RS::Set();
		Write(c);
	}

	static bool Busy()
	{
		RS::Clear();
		return Read() & 0x80;
	}
	
protected:
	static void Strobe()//__attribute__ ((noinline))
	{
		E::Set();
		Delay();
		E::Clear();
		Delay();
	}

	static void Write(uint8_t c)//__attribute__ ((noinline))
	{
		RW::Clear();
		DATA_BUS::SetDataWrite();
		DATA_BUS::Write4(c);
		Strobe();
		DATA_BUS::Write4(c<<4); 
		Strobe();
	}

	static uint8_t Read() //__attribute__ ((noinline))
	{
		RW::Set();
		DATA_BUS::SetDataRead();
		E::Set();
		//Delay();
		uint8_t res = DATA_BUS::Read4() << 4;
		E::Clear();
		Delay();
		E::Set();
		res |= DATA_BUS::Read4();
		E::Clear();
		RW::Clear();
		return res;
	}
};


#endif
