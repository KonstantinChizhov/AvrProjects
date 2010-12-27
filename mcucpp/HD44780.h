

//*****************************************************************************
//
// Title		: C++ LCD HD44780 compatible display
// Author		: Konstantin Chizhov
// Date			: 2010
// Target MCU	: platform independent
//
//       This code is distributed under the GNU Public License
//       which can be found at http://www.gnu.org/licenses/gpl.txt
//
// As a special exception, you may use this file as part of a free software
// library without restriction. Specifically, if other files instantiate
// templates or use macros or inline functions from this file, or you compile
// this file and link it with other files to produce an executable, this
// file does not by itself cause the resulting executable to be covered by
// the GNU General Public License. This exception does not however
// invalidate any other reasons why the executable file might be covered by
// the GNU General Public License.
//*****************************************************************************

#ifndef HD44780_HPP
#define HD44780_HPP

#include "static_assert.h"

class LcdBase
{
	protected:
	LcdBase(){}
	static void Delay()
	{
		_delay_us(200); 
	}
};



template<class BUS, uint8_t LINE_WIDTH=8, uint8_t LINES=2>
class Lcd: public LcdBase
{
	typedef typename BUS::template Pin<0> RS;
	typedef typename BUS::template Pin<1> RW;
	typedef typename BUS::template Pin<2> E;
	typedef typename BUS::template Slice<3, 4> DATA_BUS;

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
		BUS::SetConfiguration(BUS::Out);
		RW::Clear();
		RS::Clear();
		DATA_BUS::Write(0x03<<3); 
		Strobe();
		Strobe();
		Strobe();
		_delay_ms(60);
		DATA_BUS::Write(0x02<<3); // set 4 bit mode 
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
		RS::Clear();
		Write(0x01); 
		Write(0x02);
	}

	static void Goto(uint8_t pos)
	{
		RS::Clear();
		Write(0x80+pos);
	}

	static void Goto(uint8_t x, uint8_t y)
	{
		RS::Clear();
		if(y == 1)
			x += 0x40;
		Write(0x80+x);
	}

	static void Home(void)
	{
		RS::Clear();
		Write(0x02);
	}

	static void Puts(const char *s, uint8_t len)
	{
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
		DATA_BUS::SetConfiguration(DATA_BUS::Out);
		DATA_BUS::Write(c>>1);
		Strobe();
		DATA_BUS::Write(c<<3); 
		Strobe();
	}

	static uint8_t Read() //__attribute__ ((noinline))
	{
		DATA_BUS::SetConfiguration(DATA_BUS::In);
		RW::Set();
		E::Set();
		//Delay();
		uint8_t res = DATA_BUS::Read() << 4;
		E::Clear();
		Delay();
		E::Set();
		res |= DATA_BUS::Read();
		E::Clear();
		RW::Clear();
		return res;
	}
};


#endif
