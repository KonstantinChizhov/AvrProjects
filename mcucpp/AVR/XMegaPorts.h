#pragma once

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

#ifndef IOPORTS_HPP
#error "Do not include this file directly. Use ioports.h instead."
#endif

#ifndef XMEGA_PORTS_H
#define XMEGA_PORTS_H
//Port definitions for XMega A families.

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
			enum{Width=sizeof(DataT)*8};
		public:
			/*TODO: Implement other configurations*/
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

#define MAKE_PORT(portName, className, ID) \
	class className: public NativePortBase{\
	public:\
		typedef uint8_t DataT;\
	public:\
		static void Write(DataT value)\
		{\
			portName.OUT = value;\
		}\
		static void ClearAndSet(DataT clearMask, DataT value)\
		{\
			Clear(clearMask);\
			Set(value);\
		}\
		static DataT Read()\
		{\
			return portName.OUT;\
		}\
		static void Set(DataT value)\
		{\
			portName.OUTSET = value;\
		}\
		static void Clear(DataT value)\
		{\
			portName.OUTCLR = value;\
		}\
		static void Toggle(DataT value)\
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
		template<unsigned pin>\
		static void SetPinConfiguration(Configuration configuration)\
		{\
			BOOST_STATIC_ASSERT(pin < Width);\
			if(configuration)\
				portName.DIRSET = 1 << pin;\
			else\
				portName.DIRCLR = 1 << pin;\
		}\
		static void SetConfiguration(DataT mask, Configuration configuration)\
		{\
			if(configuration)\
				portName.DIRSET = mask;\
			else\
				portName.DIRCLR = mask;\
		}\
		enum{Id = ID};\
		enum{Width=8};\
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


#define MAKE_VIRTUAL_PORT(portName, className, VPCTRL, VPMAP_enum, ID) \
	class className{\
	public:\
		typedef uint8_t DataT;\
	public:\
		enum PinConfiguration\
		{\
			AnalogIn = 0,\
			In = 0x00,\
			PullUpOrDownIn = 0x00,\
			Out = 0x01,\
			AltOut = 0x01,\
		};\
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
			portName.OUT |= value;\
		}\
		static void Clear(DataT value)\
		{\
			portName.OUT &= ~value;\
		}\
		static void Toggle(DataT value)\
		{\
			portName.OUT ^= value;\
		}\
		static void DirSet(DataT value)\
		{\
			portName.DIR |= value;\
		}\
		static void DirClear(DataT value)\
		{\
			portName.DIR &= ~value;\
		}\
		static DataT PinRead()\
		{\
			return portName.IN;\
		}\
		static void DirToggle(DataT value)\
		{\
			portName.DIR ^= value;\
		}\
		enum{Id = ID};\
		enum{Width=8};\
		static void Map(VPMAP_enum value)\
		{\
			VPCTRL |= value;\
		}\
	};

#ifndef __ICCAVR__//PORTCFG_VP0MAP_t is undefined for some strange reason
#ifdef USE_VPORT0
	MAKE_VIRTUAL_PORT(VPORT0, VPort0, PORTCFG.VPCTRLA, PORTCFG_VP0MAP_t, 'V0')
#endif

#ifdef USE_VPORT1
	MAKE_VIRTUAL_PORT(VPORT1, VPort1, PORTCFG.VPCTRLA, PORTCFG_VP1MAP_t, 'V1')
#endif

#ifdef USE_VPORT2
	MAKE_VIRTUAL_PORT(VPORT2, VPort2, PORTCFG.VPCTRLB, PORTCFG_VP2MAP_t, 'V2')
#endif

#ifdef USE_VPORT3
	MAKE_VIRTUAL_PORT(VPORT3, VPort3, PORTCFG.VPCTRLB, PORTCFG_VP3MAP_t, 'V3')
#endif
#endif     
}//namespace IO

#endif
