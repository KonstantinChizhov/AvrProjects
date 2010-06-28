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
//*****************************************************************************

#pragma once
#include <avr/io.h>
#include "iopin.h"

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
			template<uint8_t PIN>\
			class Pin :public TPin<className, PIN>\
			{};\
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
