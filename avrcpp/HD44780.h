

//*****************************************************************************
//
// Title		: C++ LCD HD44780 compatible display
// Author		: Konstantin Chizhov
// Date			: 2010
// Target MCU	: Atmel mega AVR, TINY avr AND Xmega Series
//
//       This code is distributed under the GNU Public License
//       which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#ifndef HD44780_HPP
#define HD44780_HPP

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



template<class RS, class RW, class E, class DATA_BUS, uint8_t LINE_WIDTH=8, uint8_t LINES=2>
class Lcd: public LcdBase
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
		DATA_BUS::DirSet(0x0f);
		RW::Clear();
		RS::Clear();
		DATA_BUS::Write(0x03); 
		Strobe();
		Strobe();
		Strobe();
		_delay_ms(60);
		DATA_BUS::Write(0x02); // set 4 bit mode 
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
		DATA_BUS::DirSet(0x0f);
		DATA_BUS::Write(c>>4);
		Strobe();
		DATA_BUS::Write(c); 
		Strobe();
	}

	static uint8_t Read() //__attribute__ ((noinline))
	{
		DATA_BUS::DirSet(0);
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
