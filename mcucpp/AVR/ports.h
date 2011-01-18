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

#include "atomic.h"

namespace IO
{

	class NativePortBase :public GpioBase
	{
		public:
			typedef uint8_t DataT;
			typedef NativePortBase Base;
			enum{Width=sizeof(DataT)*8};
			static const unsigned MaxBitwiseOutput = 5;
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
			
			template<GenericConfiguration config>
			struct MapConfigurationConst
			{
				static const Configuration value = In;
			};
	};
	

	template<> struct NativePortBase::MapConfigurationConst<GpioBase::Out>{static const Configuration value = Out;};
	template<> struct NativePortBase::MapConfigurationConst<GpioBase::OpenDrainOut>{static const Configuration value = Out;};
	template<> struct NativePortBase::MapConfigurationConst<GpioBase::AltOut>{static const Configuration value = Out;};
	template<> struct NativePortBase::MapConfigurationConst<GpioBase::AltOpenDrain>{static const Configuration value = Out;};
	
	//Port definitions for AtTiny, AtMega families.

		template<class Port>
		class PortImplimentation :public NativePortBase
		{
			template<DataT value, DataT mask>
			static inline void SetBitWise()
			{
				if(mask == 0) return;
				if(value & mask)
					Port::PortSet(Port::Port() | (value & mask));
				SetBitWise<value, mask << 1>();
			}

			template<DataT value, DataT mask>
			static inline void ClearBitWise()
			{
				if(mask == 0) return;
				if(value & mask)
					Port::PortSet(Port::Port() & ~(value & mask));
				ClearBitWise<value, mask << 1>();
			}
		public:
			static void Write(DataT value)
			{
				Port::PortSet(value);
			}

			static void ClearAndSet(DataT clearMask, DataT value)
			{
				ATOMIC Port::PortSet((Port::Port() & ~clearMask) | value);
			}

			static DataT Read()
			{
				return Port::Port();
			}

			static void Set(DataT value)
			{
				ATOMIC Port::PortSet(Port::Port() | value);
			}

			static void Clear(DataT value)
			{
				ATOMIC Port::PortSet(Port::Port() & ~value);
			}

			static void Toggle(DataT value)
			{
				ATOMIC Port::PortSet(Port::Port() ^ value);
			}

			static DataT PinRead()
			{
				return Port::Pin();
			}

			// constant interface

			template<DataT clearMask, DataT value>
			static void ClearAndSet()
			{
				const DataT bitsToSet = value & clearMask;
				const DataT bitsToClear = ~value & clearMask;

				const unsigned countBitsToChange = PopulatedBits<clearMask>::value;

				if(countBitsToChange <= MaxBitwiseOutput)
				{
					SetBitWise<bitsToSet, 1>();
					ClearBitWise<bitsToClear, 1>();
				}
				else
					ATOMIC Port::PortSet((Port::Port() & ~clearMask) | value);
			}

			template<DataT value>
			static void Toggle()
			{
				ATOMIC Port::PortSet(Port::Port() ^ value);
			}

			template<DataT value>
			static void Set()
			{
				if(PopulatedBits<value>::value <= MaxBitwiseOutput)
					SetBitWise<value, 1>();
				else
					ATOMIC Port::PortSet(Port::Port() | value);
			}

			template<DataT value>
			static void Clear()
			{
				if(PopulatedBits<value>::value <= MaxBitwiseOutput)
					ClearBitWise<value, 1>();
				else
					ATOMIC Port::PortSet(Port::Port() & ~value);
			}

			
			// Configuration interface

			template<unsigned pin>
			static void SetPinConfiguration(Configuration configuration)
			{
				BOOST_STATIC_ASSERT(pin < Width);
				if(configuration)
					Port::DirSet(Port::Dir() | 1 << pin);
				else
					Port::DirSet(Port::Dir() & ~(1 << pin));
			}

			static void SetConfiguration(DataT mask, Configuration configuration)
			{
				if(configuration)
					Port::DirSet(Port::Dir() | mask);
				else
					Port::DirSet(Port::Dir() & ~mask);
			}

			template<DataT mask, Configuration configuration>
			static void SetConfiguration()
			{
				if(configuration)
					Port::DirSet(Port::Dir() | mask);
				else
					Port::DirSet(Port::Dir() & ~mask);
			}
		};

	#define MAKE_PORT(portName, ddrName, pinName, className, ID) \
			class className :public PortImplimentation<className>{\
			public:\
				using PortImplimentation<className>::DataT;\
			protected:\
				static DataT Port(){return portName;}\
				static void PortSet(DataT value){portName = value;}\
				static DataT Dir(){return ddrName;}\
				static void DirSet(DataT value){ddrName = value;}\
				static DataT Pin(){return pinName;}\
				friend class PortImplimentation<className>;\
			public:\
				enum{Id = ID};\
			};\

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
