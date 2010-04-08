#pragma once

#ifndef PORTS_HPP
#error "Do not include this file directly. Use ports.h instead."
#endif

//Port definitions for XMega A families.

#define MAKE_PORT(portName, className, ID) \
	class className{\
	public:\
		typedef uint8_t DataT;\
	public:\
		static void Write(DataT value)\
		{\
			portName.OUT = value;\
		}\
		static DataT Read()\
		{\
			return portName.OUT;\
		}\
		static void DirWrite(DataT value)\
		{\
			portName.DIR = value;\
		}\
		static DataT DirRead()\
		{\
			return portName.DIR;\
		}\
		static void Set(DataT value)\
		{\
			portName.OUTSET = value;\
		}\
		static void Clear(DataT value)\
		{\
			portName.OUTCLR = value;\
		}\
		static void Togle(DataT value)\
		{\
			portName.OUTTGL = value;\
		}\
		static void DirSet(DataT value)\
		{\
			portName.DIRSET = value;\
		}\
		static void DirClear(DataT value)\
		{\
			portName.DIRCLR = value;\
		}\
		static DataT PinRead()\
		{\
			return portName.IN;\
		}\
		static void DirTogle(DataT value)\
		{\
			portName.DIRTGL = value;\
		}\
		enum{Id = ID};\
		enum{Width=8};\
	};


#ifdef PORTA
MAKE_PORT(PORTA, Porta, 'A')
#endif


#ifdef PORTB
MAKE_PORT(PORTB, Portb, 'B')
#endif


#ifdef PORTC
MAKE_PORT(PORTC, Portc, 'C')
#endif


#ifdef PORTD
MAKE_PORT(PORTD, Portd, 'D')
#endif


#ifdef PORTE
MAKE_PORT(PORTE, Porte, 'E')
#endif


#ifdef PORTF
MAKE_PORT(PORTF, Portf, 'F')
#endif

#ifdef PORTG
MAKE_PORT(PORTG, Portg, 'G')
#endif

#ifdef PORTH
MAKE_PORT(PORTH, Porth, 'H')
#endif

#ifdef PORTJ
MAKE_PORT(PORTJ, Portj, 'J')
#endif

#ifdef PORTK
MAKE_PORT(PORTK, Portk, 'K')
#endif

#ifdef PORTQ
MAKE_PORT(PORTQ, Portq, 'Q')
#endif

#ifdef PORTR
MAKE_PORT(PORTR, Portr, 'R')
#endif

