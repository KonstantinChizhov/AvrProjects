#pragma once

#include "static_assert.h"

template<int SIZE, class DATA_T=unsigned char>
class RingBuffer
{
public:
	typedef uint8_t INDEX_T;
private:
	BOOST_STATIC_ASSERT((SIZE&(SIZE-1))==0);//SIZE must be a power of 2
	DATA_T _data[SIZE];
    volatile INDEX_T _readCount;
    volatile INDEX_T _writeCount;
	static const INDEX_T _mask = SIZE - 1;
public:

	inline uint8_t Write(DATA_T value)
	{
		if(IsFull())
			return 0;
		_data[_writeCount++ & _mask] = value;
		return true;
	}

	inline uint8_t Read(DATA_T &value)
	{
		if(IsEmpty())
			return 0;
		value = _data[_readCount++ & _mask];
		return 1;
	}

	inline DATA_T First()const
	{
		return operator[](0);
	}

	inline DATA_T Last()const
	{
		return operator[](Size());
	}

	inline DATA_T& operator[] (INDEX_T i)
	{
		if(IsEmpty())
			return DATA_T();
		return _data[(_readCount + i) & _mask];
	}

	inline const DATA_T operator[] (INDEX_T i)const
	{
		if(IsEmpty())
			return DATA_T();
		return _data[(_readCount + i) & _mask];
	}

	inline uint8_t IsEmpty()const
	{
		return _writeCount == _readCount;
	}
		
	inline uint8_t IsFull()const
	{
		return ((INDEX_T)(_writeCount - _readCount) & (INDEX_T)~(_mask)) != 0;
	}

	INDEX_T Count()const
	{
		return (_writeCount - _readCount) & _mask;
	}

	inline void Clear()
	{
		_readCount=0;
    	_writeCount=0;
	}

	inline unsigned Size()
	{return SIZE;}
};
