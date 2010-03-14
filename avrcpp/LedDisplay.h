#ifndef LED_HPP
#define LED_HPP
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
class SegmentsInv
{
public:
	void Write(uint8_t value)
	{
		value=~value;
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

template<class PORT>
class PortSegments
{
public:
	void Write(uint8_t value)
	{
		PORT::dir() = 0xff;
		PORT::Write(value);
	}
};

template<class PORT>
class PortSegmentsInv
{
public:
	void Write(uint8_t value)
	{
		PORT::dir() = 0xff;
		PORT::Write(~value);
	}
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


class LedMapTable
{
public:
	uint8_t Map(uint8_t bcd)
	{
		return pgm_read_byte(PSTR("\x3f\x06\x5b\x4f\x66\x6d\x7d\x7\x7f\x6f\x77\x7c\x39\x5e\x79\x71")+bcd);
	}
};

template<class SEGMENTS, class COMMONS, class MAP_TABLE=LedMapTable>
class LedDisplay
{
public:
	LedDisplay()
	{
		position = 0;
	}

	void WriteDec(uint16_t value)
	{
		for(uint8_t i = 0; i < NumDidgits; i++)
		{
			div_t res = div(value, 10);
			_value[NumDidgits-i-1] = _table.Map(res.rem);
			value = res.quot;
			if(value==0 && res.rem==0)
			{
				_value[NumDidgits-i-1] = 0;
			}
		}
	}

	void WriteDec(uint32_t value)
	{
		for(uint8_t i = 0; i < NumDidgits; i++)
		{
			ldiv_t res = ldiv(value, 10);
			_value[NumDidgits-i-1] = _table.Map(res.rem);
			value = res.quot;
			if(value==0 && res.rem==0)
			{
				_value[NumDidgits-i-1] = 0;
			}
		}
	}

	void WriteHex(uint16_t value)
	{
		for(uint8_t i = 0; i < NumDidgits; i++)
		{
			uint8_t rem = value & 0x0f;
			_value[NumDidgits-i-1] = _table.Map(rem);
			value >>= 4;
			if(value==0 && rem==0)
			{
				_value[NumDidgits-i-1] = 0;
			}	
		}
	}

	void WriteHex(uint32_t value)
	{
		for(uint8_t i = 0; i < NumDidgits; i++)
		{
			uint8_t rem = value & 0x0f;
			_value[NumDidgits-i-1] = _table.Map(rem);
			value >>= 4;
			if(value==0 && rem==0)
			{
				_value[NumDidgits-i-1] = 0;
			}	
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
