#pragma once

#ifndef IOPORTS_HPP
#error "Do not include this file directly. Use ioports.h instead."
#endif

#ifndef STM32_PORTS_H
#define STM32_PORTS_H

namespace IO
{

#define MAKE_PORT(CRL, CRH, IDR, ODR, BSRR, BRR, LCKR, className, ID) \
	class className{\
	public:\
		typedef uint16_t DataT;\
	public:\
		static void Write(DataT value)\
		{\
			ODR = value;\
		}\
		static void ClearAndSet(DataT clearMask, DataT value)\
		{\
			BSRR = value | (uint32_t)clearMask << 16;\
		}\
		static void DirClearAndSet(DataT clearMask, DataT value)\
		{\
			DirClear(clearMask);\
			DirSet(value);\
		}\
		static DataT Read()\
		{\
			return ODR;\
		}\
		static void DirWrite(DataT value)\
		{\
			\
		}\
		static DataT DirRead()\
		{\
			return ;\
		}\
		static void Set(DataT value)\
		{\
			BSRR = value;\
		}\
		static void Clear(DataT value)\
		{\
			BSRR = (uint32_t)value << 16;\
		}\
		static void Toggle(DataT value)\
		{\
			ODR ^= value;\
		}\
		static void DirSet(DataT value)\
		{\
			\
		}\
		static void DirClear(DataT value)\
		{\
			\
		}\
		static DataT PinRead()\
		{\
			return IDR;\
		}\
		static void DirTogle(DataT value)\
		{\
			\
		}\
		enum{Id = ID};\
		enum{Width=16};\
	};


#ifdef USE_PORTA
MAKE_PORT(PORTA, Porta, 'A')
#endif


#ifdef USE_PORTB
MAKE_PORT(PORTB, Portb, 'B')
#endif


#ifdef USE_PORTC
MAKE_PORT(PORTC, Portc, 'C')
#endif


#ifdef USE_PORTD
MAKE_PORT(PORTD, Portd, 'D')
#endif


#ifdef USE_PORTE
MAKE_PORT(PORTE, Porte, 'E')
#endif


#ifdef USE_PORTF
MAKE_PORT(PORTF, Portf, 'F')
#endif

#ifdef USE_PORTG
MAKE_PORT(PORTG, Portg, 'G')
#endif

#ifdef USE_PORTH
MAKE_PORT(PORTH, Porth, 'H')
#endif

#ifdef USE_PORTJ
MAKE_PORT(PORTJ, Portj, 'J')
#endif

#ifdef USE_PORTK
MAKE_PORT(PORTK, Portk, 'K')
#endif

#ifdef USE_PORTQ
MAKE_PORT(PORTQ, Portq, 'Q')
#endif

#ifdef USE_PORTR
MAKE_PORT(PORTR, Portr, 'R')
#endif

}//namespace IO

#endif
