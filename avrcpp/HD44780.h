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

	void SetDataRead()
	{
		d4.SetDirRead();
		d5.SetDirRead();
		d6.SetDirRead();
		d7.SetDirRead();
	}

	void SetDataWrite()
	{
		d4.SetDirWrite();
		d5.SetDirWrite();
		d6.SetDirWrite();
		d7.SetDirWrite();
	}

	void Write4(uint8_t c) //4 msb
	{
		d4.Set(c&16);
		d5.Set(c&32);
		d6.Set(c&64);
		d7.Set(c&128);
	}

	uint8_t Read4()
	{
		uint8_t res = 0;
		if(d4.IsSet())
			res|=(1);
		if(d5.IsSet())
			res|=(1<<1);
		if(d6.IsSet())
			res|=(1<<2);
		if(d7.IsSet())
			res|=(1<<3);
		return res;
	}

	D4 d4;
	D5 d5;
	D6 d6;
	D7 d7;
};

template<class PORT, uint8_t DATA_BUS_OFFSET>
class PortLcdDataBus
{

protected:
	
	BOOST_STATIC_ASSERT(DATA_BUS_OFFSET < 5);

	enum{PortMask = 0x0f << DATA_BUS_OFFSET};

	PortLcdDataBus(){}

	void SetDataRead()
	{
		PORT::dir() &= (uint8_t)~PortMask;
	}

	void SetDataWrite()
	{
		PORT::dir() |= (uint8_t)PortMask;
	}

	void Write4(uint8_t c) //4 msb
	{
		PORT::data() = (PORT::data() & (uint8_t)~PortMask) | (c >> (4-DATA_BUS_OFFSET));
	}

	uint8_t Read4()
	{
		return (PORT::pin() & PortMask) >> DATA_BUS_OFFSET;
	}
};


template<class RS, class RW, class E, class DATA_BUS, uint8_t LINE_WIDTH=8, uint8_t LINES=2>
class Lcd: public LcdBase, DATA_BUS
{
public:
	uint8_t LineWidth()
	{
		return LINE_WIDTH;
	}

	uint8_t Lines()
	{
		return LINES;
	}

	Lcd()
	{
		e.SetDirWrite();
		rw.SetDirWrite();
		rs.SetDirWrite();
		DATA_BUS::SetDataWrite();
		rw.Clear();
		rs.Clear();
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

	void Clear(void)
	{
		while(Busy());
		rs.Clear();
		Write(0x01); 
		while(Busy());
		Write(0x02);
	}

	void Goto(uint8_t pos)
	{
		while(Busy());
		rs.Clear();
		Write(0x80+pos);
	}

	void Goto(uint8_t x, uint8_t y)
	{
		while(Busy());
		rs.Clear();
		if(y == 1)
			x += 0x40;
		Write(0x80+x);
	}

	void Home(void)
	{
		while(Busy());
		rs.Clear();
		Write(0x02);
	}

	void Puts(const char *s, uint8_t len)
	{
		while(Busy());
		rs.Set(); // write characters 
		while(len-- && *s){
			Write(*s++);
		} 
	}

	void Putch(char c)
	{
		rs.Set();
		Write(c);
	}

	bool Busy()
	{
		rs.Clear();
		return Read() & 0x80;
	}
	
protected:
	void Strobe()//__attribute__ ((noinline))
	{
		e.Set();
		Delay();
		e.Clear();
		Delay();
	}

	void Write(uint8_t c)//__attribute__ ((noinline))
	{
		rw.Clear();
		DATA_BUS::SetDataWrite();
		DATA_BUS::Write4(c);
		Strobe();
		DATA_BUS::Write4(c<<4); 
		Strobe();
	}

	uint8_t Read() //__attribute__ ((noinline))
	{
		rw.Set();
		DATA_BUS::SetDataRead();
		e.Set();
		//Delay();
		uint8_t res = DATA_BUS::Read4() << 4;
		e.Clear();
		Delay();
		e.Set();
		res |= DATA_BUS::Read4();
		e.Clear();
		rw.Clear();
		return res;
	}

	E e;
	RW rw;
	RS rs;

};


#endif
