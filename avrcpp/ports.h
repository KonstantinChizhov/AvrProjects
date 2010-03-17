#pragma once

#ifndef PORTS_HPP
#define PORTS_HPP
#include <avr/io.h>

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
	

	void WaiteForSet()
	{
		while(IsSet()==0){}
	}

	void WaiteForClear()
	{
		while(IsSet()){}
	}
}; 

template<class HI, class LOW>
class PowerDriver
{
public:
	PowerDriver()
	{
		hi.SetDirWrite();
		low.SetDirWrite();
	}
	void SetHight()
	{
		hi.Set();
		low.Set();
	}

	void SetLow()
	{
		low.Clear();
		hi.Clear();
	}

	void Free()
	{
		low.Clear();
		hi.Set();
	}
HI hi;
LOW low;
};


#endif
