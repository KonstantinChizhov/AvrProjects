//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2010
// All rights reserved.

// Redistribution and use in source and binary forms, with or without modification, 
// are permitted provided that the following conditions are met:
// Redistributions of source code must retain the above copyright notice, 
// this list of conditions and the following disclaimer.

// Redistributions in binary form must reproduce the above copyright notice, 
// this list of conditions and the following disclaimer in the documentation and/or 
// other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY 
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//*****************************************************************************



#pragma once

#ifndef IOPORTS_HPP
#error "Do not include this file directly. Use ioports.h instead."
#endif

#ifndef AVR_PORTS_H
#define AVR_PORTS_H

#ifdef PORTA
#define USE_PORTA
#endif

#ifdef PORTB
#define USE_PORTB
#endif


#ifdef PORTC
#define USE_PORTC
#endif

#ifdef PORTD
#define USE_PORTD
#endif

#ifdef PORTE
#define USE_PORTE
#endif

#ifdef PORTF
#define USE_PORTF
#endif

#ifdef PORTG
#define USE_PORTG
#endif

#ifdef PORTH
#define USE_PORTH
#endif

#ifdef PORTJ
#define USE_PORTJ
#endif

#ifdef PORTK
#define USE_PORTK
#endif

#ifdef PORTQ
#define USE_PORTQ
#endif

#ifdef PORTR
#define USE_PORTR
#endif

#ifdef VPORT0
#define USE_VPORT0
#endif

#ifdef VPORT1
#define USE_VPORT1
#endif

#ifdef VPORT2
#define USE_VPORT2
#endif

#ifdef VPORT3
#define USE_VPORT3
#endif

namespace IO
{

	class NativePortBase :public GpioBase
	{
		public:
			typedef uint8_t DataT;
			typedef NativePortBase Base;
			enum{Width=sizeof(DataT)*8};
		public:
			enum Configuration
			{
				AnalogIn = 0,
				In = 0x00,
				PullUpOrDownIn = 0x00,
				Out = 0x01,
				AltOut = 0x01,
			};
			
			static Configuration MapConfiguration(GenericConfiguration config)
			{
				if(config & GpioBase::Out)
					return Out;
				return In;
			}
	};
	
	//Port definitions for AtTiny, AtMega families.

	#define MAKE_PORT(portName, ddrName, pinName, className, ID) \
		class className :public NativePortBase{\
		public:\
			static void Write(DataT value)\
			{\
				portName = value;\
			}\
			static void ClearAndSet(DataT clearMask, DataT value)\
			{\
				portName = (portName & ~clearMask) | value;\
			}\
			static DataT Read()\
			{\
				return portName;\
			}\
			static void Set(DataT value)\
			{\
				portName |= value;\
			}\
			static void Clear(DataT value)\
			{\
				portName &= ~value;\
			}\
			static void Toggle(DataT value)\
			{\
				portName ^= value;\
			}\
			static DataT PinRead()\
			{\
				return pinName;\
			}\
			template<unsigned pin>\
			static void SetPinConfiguration(Configuration configuration)\
			{\
				BOOST_STATIC_ASSERT(pin < Width);\
				if(configuration)\
					ddrName |= 1 << pin;\
				else\
					ddrName &= ~(1 << pin);\
			}\
			static void SetConfiguration(DataT mask, Configuration configuration)\
			{\
				if(configuration)\
					ddrName |= mask;\
				else\
					ddrName &= ~mask;\
			}\
			enum{Id = ID};\
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
