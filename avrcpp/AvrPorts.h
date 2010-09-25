//*****************************************************************************
//
// Title		: C++ IO for ATMEL AVR
// Author		: Konstantin Chizhov
// Date			: 2010
// Target MCU	: Atmel mega AVR, TINY avr AND Xmega Series
//
//       This code is distributed under the GNU Public License
//       which can be found at http://www.gnu.org/licenses/gpl.txt
//
// As a special exception, you may use this file as part of a free software
// library without restriction. Specifically, if other files instantiate
// templates or use macros or inline functions from this file, or you compile
// this file and link it with other files to produce an executable, this
// file does not by itself cause the resulting executable to be covered by
// the GNU General Public License. This exception does not however
// invalidate any other reasons why the executable file might be covered by
// the GNU General Public License.
//*****************************************************************************

#pragma once

#ifndef IOPORTS_HPP
#error "Do not include this file directly. Use ioports.h instead."
#endif

#ifndef AVR_PORTS_H
#define AVR_PORTS_H

namespace IO
{

	//Port definitions for AtTiny, AtMega families.

	#define MAKE_PORT(portName, ddrName, pinName, className, ID) \
		class className{\
		public:\
			typedef uint8_t DataT;\
		public:\
			static void Write(DataT value)\
			{\
				portName = value;\
			}\
			static void ClearAndSet(DataT clearMask, DataT value)\
			{\
				portName = (portName & ~clearMask) | value;\
			}\
			static void DirClearAndSet(DataT clearMask, DataT value)\
			{\
				ddrName = (ddrName & ~clearMask) | value;\
			}\
			static DataT Read()\
			{\
				return portName;\
			}\
			static void DirWrite(DataT value)\
			{\
				ddrName = value;\
			}\
			static DataT DirRead()\
			{\
				return ddrName;\
			}\
			static void Set(DataT value)\
			{\
				portName |= value;\
			}\
			static void Clear(DataT value)\
			{\
				portName &= ~value;\
			}\
			static void Togle(DataT value)\
			{\
				portName ^= value;\
			}\
			static void DirSet(DataT value)\
			{\
				ddrName |= value;\
			}\
			static void DirClear(DataT value)\
			{\
				ddrName &= ~value;\
			}\
			static void DirTogle(DataT value)\
			{\
				ddrName ^= value;\
			}\
			static DataT PinRead()\
			{\
				return pinName;\
			}\
			enum{Id = ID};\
			enum{Width=sizeof(DataT)*8};\
		};

	#ifdef USE_PORTA
	MAKE_PORT(PORTA, DDRA, PINA, Porta, 'A')
	#endif


	#ifdef USE_PORTB
	MAKE_PORT(PORTB, DDRB, PINB, Portb, 'B')
	#endif


	#ifdef USE_PORTC
	MAKE_PORT(PORTC, DDRC, PINC, Portc, 'C')
	#endif


	#ifdef USE_PORTD
	MAKE_PORT(PORTD, DDRD, PIND, Portd, 'D')
	#endif


	#ifdef USE_PORTE
	MAKE_PORT(PORTE, DDRE, PINE, Porte, 'E')
	#endif


	#ifdef USE_PORTF
	MAKE_PORT(PORTF, DDRF, PINF, Portf, 'F')
	#endif

	#ifdef USE_PORTG
	MAKE_PORT(PORTG, DDRG, PING, Portg, 'G')
	#endif

}

#endif
