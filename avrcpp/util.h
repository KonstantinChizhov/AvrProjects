
#ifndef UTIL_HPP
#define UTIL_HPP

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

template<int num, int pow> 
struct Pow 
{
	enum { value = num * Pow<num, pow-1>::value };
};

template<int num> 
struct Pow<num, 0> 
{
	enum { value = 1 };
};

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
		uint8_t c;
		while(!DATA_SOURCE::Getch(c));
		return c;
	}
};

template<class DATA_SOURCE, uint8_t fieldSize=8>
class TextFormater :public DATA_SOURCE
{
public:

	TextFormater<DATA_SOURCE>& operator<< (int value)
	{
		char buffer[7];
		itoa(value, buffer, 10);
		Puts(buffer);
		return *this;
	}

	TextFormater<DATA_SOURCE>& operator<< (long value)
	{
		char buffer[11];
		ltoa(value, buffer, 10);
		Puts(buffer);
		return *this;
	}

	TextFormater<DATA_SOURCE>& operator<< (unsigned long value)
	{
		char buffer[11];
		ultoa(value, buffer, 10);
		Puts(buffer);
		return *this;
	}
	
	TextFormater<DATA_SOURCE>& operator<< (unsigned value)
	{
		char buffer[7];
		utoa(value, buffer, 10);
		Puts(buffer);
		return *this;
	}

	TextFormater<DATA_SOURCE>& operator<< (double value)
	{
		const uint8_t presision=2;
		char buffer[fieldSize+1];
		if(value < Pow<10, fieldSize - presision - 1>::value - 0.1)
		{
			dtostrf(value, fieldSize - presision - 1, presision, buffer);
		}
		else
		{
			dtostre(value, buffer, fieldSize - 6, 0);
		}
		Puts(buffer);
		return *this;
	}

	TextFormater<DATA_SOURCE>& operator<< (const char *value)
	{
		Puts(value);
		return *this;
	}

	void Write(const void *data, uint16_t size)
	{
		for(uint16_t i=0; i<size; ++i)
		{
			DATA_SOURCE::Putch(((uint8_t*)data)[i]);
		}
	}

	void WriteP(const void *data, uint16_t size)
	{
		uint8_t *ptr=(uint8_t*)data;
		for(uint16_t i=0; i<size; ++i)
		{
			uint8_t c = pgm_read_byte(ptr);
			DATA_SOURCE::Putch(c);
			ptr++;
		}
	}

	void WriteF(const void *data, uint16_t size)
	{
		uint8_t *ptr=(uint8_t*)data;
		for(uint16_t i=0; i<size; ++i)
		{
			uint8_t c = eeprom_read_byte(ptr);
			DATA_SOURCE::Putch(c);
			ptr++;
		}
	}

	void Puts(const char *str)
	{
		while(*str)
		{
			DATA_SOURCE::Putch(*str);
			str++;
		}
	}

	void PutsP(const char *str)
	{
		uint8_t c;
		while((c = pgm_read_byte(str++)))
		{
			DATA_SOURCE::Putch(c);
		}
	}

	void PutsF(const char *str)
	{
		uint8_t c;
		while((c = eeprom_read_byte((uint8_t*)str++)))
		{
			DATA_SOURCE::Putch(c);
		}
	}

	void Read(void *data, uint16_t size)
	{
		for(uint16_t i=0; i<size; ++i)
		{
			DATA_SOURCE::Getch(((uint8_t*)data)[i]);
		}
	}

	void Gets(char *buffer, uint8_t len)
	{
		uint8_t c, pos=0;
		do
		{
			DATA_SOURCE::Getch(c);
			if(c=='\r')
			{
				DATA_SOURCE::Getch(c);
				break;
			}
			buffer[pos++]=c;
			if(pos>=len)break;
		}
		while(c!='\n' && c!=0);
		buffer[pos]=0;
	} 

	TextFormater<DATA_SOURCE>& operator>> (unsigned &value)
	{
		char buffer[7];
		Gets(buffer, sizeof(buffer));
		value = atoi(buffer);
		return *this;
	}

	TextFormater<DATA_SOURCE>& operator>> (unsigned long &value)
	{
		char buffer[11];
		Gets(buffer, sizeof(buffer));
		value = atol(buffer);
		return *this;
	}

	template<class T>
	void Write(T value)
	{
		Write(&value, sizeof(T));
	}

	template<class T>
	T Read()
	{
		T value;
		Read(&value, sizeof(T));
		return value;
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
	static T Read()
	{
		union
		{
			T value;
			uint8_t rawData[sizeof(T)];
		};

		Read(rawData, sizeof(T));
		return value;
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
 
