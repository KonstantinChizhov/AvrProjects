#pragma once

#ifndef PORTS_HPP
#define PORTS_HPP
#include <avr/io.h>
#include "static_assert.h"

#define MAKE_PORT(portName, ddrName, pinName, className) \
	struct className{\
		static volatile uint8_t &data()\
		{\
			return portName;\
		}\
		static volatile uint8_t &dir()\
		{\
			return ddrName;\
		}\
		static volatile uint8_t &pin()\
		{\
			return pinName;\
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
MAKE_PORT(PORTA, DDRA, PINA, Porta)
#endif


#ifdef PORTB
MAKE_PORT(PORTB, DDRB, PINB, Portb)
#endif


#ifdef PORTC
MAKE_PORT(PORTC, DDRC, PINC, Portc)
#endif


#ifdef PORTD
MAKE_PORT(PORTD, DDRD, PIND, Portd)
#endif


#ifdef PORTE
MAKE_PORT(PORTE, DDRE, PINE, Porte)
#endif


#ifdef PORTF
MAKE_PORT(PORTF, DDRF, PINF, Portf)
#endif

#ifdef PORTG
MAKE_PORT(PORTG, DDRG, PING, Portg)
#endif



template<class PORT, uint8_t PIN>
class TPin
{
	BOOST_STATIC_ASSERT(PIN < 8);
public:
	static void Set()
	{
		PORT::data() |= (1 << PIN);
	}

	static void Set(uint8_t val)
	{
		if(val)
			Set();
		else Clear();
	}

	static void Clear()
	{
		PORT::data() &= (uint8_t)~(1 << PIN);
	}

	static void Togle()
	{
		PORT::data() ^= (1 << PIN);
	}

	static void SetDirRead()
	{
		PORT::dir() &= (uint8_t)~(1 << PIN);
	}

	static void SetDirWrite()
	{
		PORT::dir() |= (1 << PIN);
	}

	static uint8_t IsSet()
	{
		return PORT::pin() & (uint8_t)(1 << PIN);
	}	
	

	static void WaiteForSet()
	{
		while(IsSet()==0){}
	}

	static void WaiteForClear()
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
