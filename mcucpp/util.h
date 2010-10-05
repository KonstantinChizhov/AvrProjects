
#ifndef UTIL_HPP
#define UTIL_HPP

//*****************************************************************************
//
// Title		: C++ utility classes and functions for ATMEL AVR
// Author		: Konstantin Chizhov
// Date			: 2010
// Target MCU	: Atmel mega AVR, TINY avr AND Xmega Series
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

#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>

union Int32
{
	uint32_t Dword;
	uint16_t Words[2];
	uint8_t Bytes[4];
};

union Int16
{
	uint16_t Word;
	uint8_t Bytes[2];
};

//integer part of decimal logarithm
template <unsigned long Arg>
struct Log10
{
        enum { value = Log10<Arg/10>::value + 1};
};

template <>
struct Log10<1>
{
        enum { value = 0 };
};

template <>
struct Log10<0>
{
        enum { value = -1 };
};


template<unsigned num, unsigned pow> 
struct Pow 
{
	enum { value = num * Pow<num, pow-1>::value };
};

template<unsigned num> 
struct Pow<num, 0> 
{
	enum { value = 1 };
};

// returns true if T is signed integral type
template<class T>
bool TestSigned()
{
	return T(-1) < 0;
}

template<class T>
T MaxValue()
{
	if(TestSigned<T>())
		return Pow<2, sizeof(T)*8-1>::value - 1;
	else
		return Pow<2, sizeof(T)*8>::value - 1;
}

//utility functions
template<class T>
T min(T v1, T v2)
{
	return v1 > v2 ? v2 : v1;
}

template<class T>
T max(T v1, T v2)
{
	return v1 < v2 ? v2 : v1;
}

//rounding to power of 2
inline uint16_t flp2 (uint16_t x)
{
	x |= (x >> 1);
	x |= (x >> 2);
	x |= (x >> 4);
	x |= (x >> 8);
//	x |= (x >> 16);
	return x - (x >> 1);
}

template<class T>
inline uint8_t ilog2 (T x)
{
    int8_t res=-1;
    do
    {
        res++;
    }while(x >>= 1);
    return res;
}

template<class T>
inline uint8_t ilog10 (T x)
{
    T p = 1;
    int8_t i;
    for (i = -1; i <= 2.408*sizeof(x); i++)
    {
        if (x < p)
            return i;
        p *= 10;
    }
    return i;
}

inline uint16_t div5(uint16_t num, uint16_t &rem)
{
    uint16_t q = num*0x6667ul >> 16;
    q >>= 1;
    rem = num - q*5;
    return q;
}

inline uint16_t div10(uint16_t num, uint16_t &rem)
{
    uint16_t q = num*0x6667ul >> 16;
    q >>= 2;
    rem = num - q*10;
    return q;
}

inline uint32_t div10(uint32_t num, uint32_t &rem)
{
    uint32_t q = num*0x66666667ull >> 32;
    q >>= 2;
    rem = num - q*10;
    return q;
}

inline uint8_t CountOfOnes(uint8_t val)
{
	val = (val & 0x55) + ((val >> 1) & 0x55);
	val = (val & 0x33) + ((val >> 2) & 0x33);
	val = (val & 0x0F) + ((val >> 4) & 0x0F);
	return val;
}

template<class DATA_SOURCE>
class WaitAdapter :public DATA_SOURCE
{
public:
	static void Write(uint8_t c)
	{
		while(!DATA_SOURCE::Putch(c));
	}

	static uint8_t Read()
	{
		//uint16_t timeout = 1000;
		uint8_t c=0;
		while(!DATA_SOURCE::Getch(c));
		return c;
	}
};


template<class DATA_SOURCE>
class BinaryFormater :public DATA_SOURCE
{
public:
	using DATA_SOURCE::Write;
	using DATA_SOURCE::Read;

	static void Write(uint32_t value)
	{
		Int32 i;
		i.Dword = value;
		DATA_SOURCE::Putch(i.Bytes[0]);
		DATA_SOURCE::Putch(i.Bytes[1]);
		DATA_SOURCE::Putch(i.Bytes[2]);
		DATA_SOURCE::Putch(i.Bytes[3]);
	}

	static void Write(uint16_t value)
	{
		DATA_SOURCE::Write(value & 0xff);
		DATA_SOURCE::Write((value >>= 8) & 0xff);
	}

	template<class T>
	static void Write(const T &value)
	{
		Write(&value, sizeof(T));
	}

	static void Write(const void *data, size_t size)
	{
		for(size_t i=0; i<size; ++i)
		{
			DATA_SOURCE::Write(((const uint8_t*)data)[i]);
		}
	}

	static void Puts(const char *str)
	{
		while(*str)
		{
			DATA_SOURCE::Write(*str);
			str++;
		}
	}

	static void Read(void *data, size_t size)
	{
		for(size_t i=0; i<size; ++i)
		{
			((uint8_t*)data)[i] = DATA_SOURCE::Read();
		}
	}

	template<class T>
	static void Read(T &value)
	{
		Read(&value, sizeof(T));
	}

	static uint32_t ReadU32()
	{
		Int32 i;
		i.Bytes[0] = DATA_SOURCE::Read();
		i.Bytes[1] = DATA_SOURCE::Read();
		i.Bytes[2] = DATA_SOURCE::Read();
		i.Bytes[3] = DATA_SOURCE::Read();
		return i.Dword;
	}

	static uint16_t ReadU16()
	{
		Int16 i;
		i.Bytes[0] = DATA_SOURCE::Read();
		i.Bytes[1] = DATA_SOURCE::Read();
		return i.Word;
	}
};

class DisableInterrupts
{
public:
	DisableInterrupts()
	{
		_sreg = SREG;
		cli();
	}
	~DisableInterrupts()
	{
		SREG = _sreg;
	}
	operator bool()
	{return false;}
private:
	uint8_t _sreg;
};

#define ATOMIC if(DisableInterrupts di = DisableInterrupts()){}else

#endif
 