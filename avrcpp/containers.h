#pragma once

#ifndef CONTAINERS_HPP
#define CONTAINERS_HPP

#include "static_assert.h"


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


#endif
