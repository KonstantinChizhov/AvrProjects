
#ifndef UTIL_HPP
#define UTIL_HPP

#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>


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

template<class DATA_SOURCE>
class WaitAdapter :public DATA_SOURCE
{
public:
	static void Putch(uint8_t c)
	{
		while(!DATA_SOURCE::Putch(c));
	}

	static void Getch(uint8_t &c)
	{
		while(!DATA_SOURCE::Getch(c));
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
	template<class T>
	BinaryFormater<DATA_SOURCE>& operator<< (const T &value)
	{
		const uint8_t * rawData = reinterpret_cast<const uint8_t *>(&value);
		for(uint8_t i=0; i<sizeof(T); ++i)
			DATA_SOURCE::Putch(rawData[i]);
		return *this;
	}

	template<class T>
	BinaryFormater<DATA_SOURCE>& operator>> (T &value)
	{
		const uint8_t * rawData = reinterpret_cast<const uint8_t *>(&value);
		for(uint8_t i=0; i<sizeof(T); ++i)
			DATA_SOURCE::Getch(rawData[i]);
		return *this;
	}

	template<class T>
	static void Write(const T &value)
	{
		const uint8_t * rawData = reinterpret_cast<const uint8_t *>(&value);
		for(uint8_t i=0; i<sizeof(T); ++i)
			DATA_SOURCE::Putch(rawData[i]);
	}

	template<class T>
	static T Read()
	{
		T value;
		uint8_t *const rawData = reinterpret_cast<uint8_t*>(&value);
		for(uint8_t i=0; i<sizeof(T); ++i)
			DATA_SOURCE::Getch(rawData[i]);
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
 
