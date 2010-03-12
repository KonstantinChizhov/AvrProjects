
#ifndef UTIL_HPP
#define UTIL_HPP

#include <stdlib.h>
#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>


namespace boost
{
	template <bool x> struct STATIC_ASSERTION_FAILURE;
	template <> struct STATIC_ASSERTION_FAILURE<true> { enum { value = 1 }; };
	template<int x> struct static_assert_test{};
}

#define BOOST_STATIC_ASSERT_BOOL_CAST(x) (bool)(x)

#define BOOST_STATIC_ASSERT( B ) \
   typedef ::boost::static_assert_test<\
      sizeof(::boost::STATIC_ASSERTION_FAILURE< BOOST_STATIC_ASSERT_BOOL_CAST( B ) >)>\
         (boost_static_assert_typedef_ ## __LINE__)


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


template<int SIZE, class DATA_T=unsigned char, class INDEX_T=unsigned char>
class RingBuffer
{
protected:
	RingBuffer()
	{
		Clear();
	}

	void Write(DATA_T c)
	{
		_data[(INDEX_T)(_writeCount++ & (INDEX_T)(SIZE-1))]=c;
	}

	DATA_T Read()
	{
		return _data[(INDEX_T)(_readCount++ & (INDEX_T)(SIZE-1))];
	}

public:

	DATA_T First()const
	{
		return operator[](0);
	}

	DATA_T Last()const
	{
		return operator[](Size());
	}

	DATA_T& operator[] (INDEX_T i)
	{
		if(IsEmpty())
			return DATA_T();
		return _data[(INDEX_T)((_readCount + i) & (INDEX_T)(SIZE-1))];
	}

	const DATA_T& operator[] (INDEX_T i)const
	{
		if(IsEmpty())
			return DATA_T();
		return _data[(INDEX_T)((_readCount + i) & (INDEX_T)(SIZE-1))];
	}

	bool IsEmpty()const
	{
		return (_writeCount == _readCount);
	}
		
	bool IsFull()const
	{
		return ((INDEX_T)(_writeCount - _readCount) & (INDEX_T)~(SIZE-1)) != 0;
	}

	void Clear()
	{
		_readCount=0;
    	_writeCount=0;
	}

	unsigned Size()
	{return SIZE;}
protected:
    DATA_T _data[SIZE];
	BOOST_STATIC_ASSERT((SIZE&(SIZE-1))==0);//SIZE_mast_be_a_power_of_two
    INDEX_T _readCount;
    INDEX_T _writeCount;   
};


template<int SIZE, class DATA_T=unsigned char, class INDEX_T=unsigned char>
class Queue :public RingBuffer<SIZE, DATA_T, INDEX_T>
{
public:
	bool Write(DATA_T c)
	{
		if(RingBuffer<SIZE, DATA_T, INDEX_T>::IsFull())
			return 0;
		RingBuffer<SIZE, DATA_T, INDEX_T>::Write(c);
		return 1;
	}

	bool Read(DATA_T &c)
	{
		if(RingBuffer<SIZE, DATA_T, INDEX_T>::IsEmpty())
			return 0;
		c=RingBuffer<SIZE, DATA_T, INDEX_T>::Read();
		return 1;
	}
};

template<int SIZE, class DATA_T=unsigned char, class INDEX_T=unsigned char>
class WrappingQueue :public RingBuffer<SIZE, DATA_T, INDEX_T>
{
public:
	bool Write(DATA_T c)
	{
		if(RingBuffer<SIZE, DATA_T, INDEX_T>::IsFull())
			RingBuffer<SIZE, DATA_T, INDEX_T>::_readCount++;
		RingBuffer<SIZE, DATA_T, INDEX_T>::Write(c);
		return 1;
	}

	bool Read(DATA_T &c)
	{
		if(RingBuffer<SIZE, DATA_T, INDEX_T>::IsEmpty())
			return 0;
		c=RingBuffer<SIZE, DATA_T, INDEX_T>::Read();
		return 1;
	}
};


template<int SIZE, class T=uint8_t, class INDEX_T=uint8_t>
class Array
{
public:
	unsigned Size()
	{return SIZE;}
	T& operator[](INDEX_T i)
	{
		return _data[i];
	}

	const T& operator[](INDEX_T i)const
	{
		return _data[i];
	}

protected:
	T _data[SIZE];
};


template<int SIZE, class T=uint8_t, class INDEX_T=uint8_t>
class Stack
{
public:
	Stack()
	{
		_index=0;
	}
	unsigned Size()
	{return SIZE;}

	void Push(T value)
	{
		if(_index>=Size()) return;
		_data[_index++] = value;
	}

	T Pop()
	{
		if(_index==0) return T();
		return _data[--_index];
	}

	T Top()
	{
		if(_index==0) return T();
		return _data[_index-1];
	}
protected:
	T _data[SIZE];
	INDEX_T _index;
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

class Pin
{
public:
	Pin(volatile unsigned char &port, uint8_t pin)
	:_port(port)
	{
		_pin = pin; 
	}

	void Set()const
	{
		_port |= (1 << _pin);
	}

	void Set(uint8_t val)const
	{
		if(val)
			Set();
		else Clear();
	}

	void Clear()const
	{
		_port &= (uint8_t)~(1 << _pin);
	}

	void Togle()const
	{
		_port ^= (1 << _pin);
	}

	void SetDirRead()const
	{
		*(&_port - 1) |= (1 << _pin);
	}

	void SetDirWrite()const
	{
		*(&_port - 1) &= (uint8_t)~(1 << _pin);
	}

	uint8_t IsSet()const
	{
		return (*(&_port - 2)) & (uint8_t)(1 << _pin);
	}

private:
	volatile unsigned char &_port; 
	uint8_t _pin;
}; 

#define MAKE_PORT(portName, className) \
	struct className{\
		static volatile uint8_t &data()\
		{\
			return portName;\
		}\
		static volatile uint8_t &dir()\
		{\
			return *(&portName - 1);\
		}\
		static volatile uint8_t &pin()\
		{\
			return *(&portName - 2);\
		}\
		static void Write(uint8_t value)\
		{\
			data() = value;\
		}\
		static uint8_t Read()\
		{\
			return data();\
		}\
	};

#ifdef PORTA
MAKE_PORT(PORTA, Porta)
#endif


#ifdef PORTB
MAKE_PORT(PORTB, Portb)
#endif


#ifdef PORTC
MAKE_PORT(PORTC, Portc)
#endif


#ifdef PORTD
MAKE_PORT(PORTD, Portd)
#endif


#ifdef PORTE
MAKE_PORT(PORTE, Porte)
#endif


#ifdef PORTF
MAKE_PORT(PORTF, Portf)
#endif


template<class PORT, uint8_t PIN>
class TPin
{
public:
	void Set()const
	{
		PORT::data() |= (1 << PIN);
	}

	void Set(uint8_t val)const
	{
		if(val)
			Set();
		else Clear();
	}

	void Clear()const
	{
		PORT::data() &= (uint8_t)~(1 << PIN);
	}

	void Togle()const
	{
		PORT::data() ^= (1 << PIN);
	}

	void SetDirRead()const
	{
		PORT::dir() &= (uint8_t)~(1 << PIN);
	}

	void SetDirWrite()const
	{
		PORT::dir() |= (1 << PIN);
	}

	uint8_t IsSet()const
	{
		return PORT::pin() & (uint8_t)(1 << PIN);
	}	
	
}; 

template<class HIGHT, class LOW>
class PowerDriver
{
public:
	PowerDriver()
	{
		height.SetDirWrite();
		low.SetDirWrite();
	}
	void SetHight()
	{
		height.Set();
		low.Set();
	}

	void SetLow()
	{
		low.Clear();
		height.Clear();
	}

	void Free()
	{
		low.Clear();
		height.Set();
	}
HIGHT height;
LOW low;
};


template<class DATA_SOURCE, uint8_t fieldSize=8>
class TextFormater :DATA_SOURCE
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
class BinaryFormater :DATA_SOURCE
{
public:
	template<class T>
	BinaryFormater<DATA_SOURCE>& operator<< (const T &value)
	{
		uint8_t *const rawData = static_cast<uint8_t *const>(&value);
		for(uint8_t i=0; i<sizeof(T); ++i)
			DATA_SOURCE::Putch(rawData[i]);
		return *this;
	}

	template<class T>
	BinaryFormater<DATA_SOURCE>& operator>> (T &value)
	{
		uint8_t *const rawData = static_cast<uint8_t *const>(&value);
		for(uint8_t i=0; i<sizeof(T); ++i)
			DATA_SOURCE::Getch(rawData[i]);
		return *this;
	}
};

#endif
 
