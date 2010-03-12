#ifndef LED_HPP
#define LED_HPP
#include <avr/io.h>
#include "util.h"

template<
	class A, 
	class B, 
	class C, 
	class D, 
	class E, 
	class F,
	class G,
	class DP
	>
class Segments
{
public:
	void Write(uint8_t value)
	{
		a.Set(value & 1);
		b.Set(value & 2);
		c.Set(value & 4);
		d.Set(value & 8);
		e.Set(value & 16);
		f.Set(value & 32);
		g.Set(value & 64);
		dp.Set(value & 128);
	}
private:
	A a; B b; C c; D d; E e; F f; G g; DP dp;
};

template<class PORT, uint8_t NUM_DIDGITS>
class CommonsPortH // active level - hi
{
	BOOST_STATIC_ASSERT(NUM_DIDGITS<=8);
public:
	CommonsPortH()
	{
		PORT::dir() |= (uint8_t)(PortMask);
	}

	~CommonsPortH()
	{
		PORT::dir() &= (uint8_t)(~PortMask);
	}
	enum{NumDidgits = NUM_DIDGITS, PortMask = Pow<2, NUM_DIDGITS>::value - 1};
	void Set(uint8_t n)
	{
		PORT::data() = (PORT::data() & (uint8_t)(~PortMask)) | (uint8_t)(1 << n);
	}
};

template<class PORT, uint8_t NUM_DIDGITS>
class CommonsPortL // active level - low
{
	BOOST_STATIC_ASSERT(NUM_DIDGITS<=8);
public:
	CommonsPortL()
	{
		PORT::dir() |= (uint8_t)(PortMask);
	}

	~CommonsPortL()
	{
		PORT::dir() &= (uint8_t)(~PortMask);
	}
	enum{NumDidgits = NUM_DIDGITS, PortMask = Pow<2, NUM_DIDGITS>::value - 1};
	void Set(uint8_t n)
	{
		PORT::data() = (PORT::data() & (uint8_t)(~PortMask)) | (uint8_t)~(1 << n);
	}
};


class LedBcdMapTable
{
public:
	uint8_t Map(uint8_t bcd)
	{
		return "\0x3f\0x06\0x5b\0x4f\0x66\0x6d\0x7d\0x7\0x7f\0x6f\0x40\0x40\0x40\0x40\0x40\0x40"[bcd];
	}
};

template<class SEGMENTS, class COMMONS, class MAP_TABLE>
class LedDisplay
{
public:
	LedDisplay()
	{
		position = 0;
	}

	void Set(uint16_t value)
	{
		for(uint8_t i=0; i<NumDidgits; i++)
		{
			_bcdValue[i] = value % 10;
			value /= 10;
			if(value==0)return;
		}
	}

	void Update()
	{
		_commons.Set(position);
		_segments.Write(_value[position++]);
		if(position >= NumDidgits)
			position = 0;
	}
	
protected:
	uint8_t position;
	enum{NumDidgits = (COMMONS::NumDidgits)};
	uint8_t _value[NumDidgits];
	SEGMENTS _segments;
	COMMONS _commons;
	MAP_TABLE _table;
};

#endif
