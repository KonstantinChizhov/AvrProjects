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

namespace IO
{

	//Port definitions for AtTiny, AtMega families.

	#define MAKE_PORT(portName, ddrName, pinName, className, ID) \
		class className{\
		public:\
			typedef uint8_t DataT;\
		private:\
			static volatile DataT &data()\
			{\
				return portName;\
			}\
			static volatile DataT &dir()\
			{\
				return ddrName;\
			}\
			static volatile DataT &pin()\
			{\
				return pinName;\
			}\
		public:\
			static void Write(DataT value)\
			{\
				data() = value;\
			}\
			static void ClearAndSet(DataT clearMask, DataT value)\
			{\
				data() = (data() & ~clearMask) | value;\
			}\
			static DataT Read()\
			{\
				return data();\
			}\
			static void DirWrite(DataT value)\
			{\
				dir() = value;\
			}\
			static DataT DirRead()\
			{\
				return dir();\
			}\
			static void Set(DataT value)\
			{\
				data() |= value;\
			}\
			static void Clear(DataT value)\
			{\
				data() &= ~value;\
			}\
			static void Togle(DataT value)\
			{\
				data() ^= value;\
			}\
			static void DirSet(DataT value)\
			{\
				dir() |= value;\
			}\
			static void DirClear(DataT value)\
			{\
				dir() &= ~value;\
			}\
			static void DirTogle(DataT value)\
			{\
				dir() ^= value;\
			}\
			static DataT PinRead()\
			{\
				return pin();\
			}\
			enum{Id = ID};\
			enum{Width=sizeof(DataT)*8};\
		};

	#ifdef PORTA
	MAKE_PORT(PORTA, DDRA, PINA, Porta, 'A')
	#endif


	#ifdef PORTB
	MAKE_PORT(PORTB, DDRB, PINB, Portb, 'B')
	#endif


	#ifdef PORTC
	MAKE_PORT(PORTC, DDRC, PINC, Portc, 'C')
	#endif


	#ifdef PORTD
	MAKE_PORT(PORTD, DDRD, PIND, Portd, 'D')
	#endif


	#ifdef PORTE
	MAKE_PORT(PORTE, DDRE, PINE, Porte, 'E')
	#endif


	#ifdef PORTF
	MAKE_PORT(PORTF, DDRF, PINF, Portf, 'F')
	#endif

	#ifdef PORTG
	MAKE_PORT(PORTG, DDRG, PING, Portg, 'G')
	#endif

}
