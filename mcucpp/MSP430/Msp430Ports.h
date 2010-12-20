//*****************************************************************************
//
// Title		: C++ IO for ATMEL AVR
// Author		: Konstantin Chizhov
// Date			: 2010
// Target MCU	: TI Msp430
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

#ifndef MSP430_PORTS_H
#define MSP430_PORTS_H

#if defined(__MSP430_HAS_PORT0__)
#define USE_PORT0
#endif

#if defined(__MSP430_HAS_PORT1__)  ||  defined(__MSP430_HAS_PORT1_R__)
#define USE_PORT1
#endif

#if defined(__MSP430_HAS_PORT2__)  ||  defined(__MSP430_HAS_PORT2_R__)
#define USE_PORT2
#endif

#if defined(__MSP430_HAS_PORT3__)  ||  defined(__MSP430_HAS_PORT3_R__)
#define USE_PORT3
#endif

#if defined(__MSP430_HAS_PORT4__)  ||  defined(__MSP430_HAS_PORT4_R__)
#define USE_PORT4
#endif

#if defined(__MSP430_HAS_PORT5__)  ||  defined(__MSP430_HAS_PORT5_R__)
#define USE_PORT5
#endif

#if defined(__MSP430_HAS_PORT6__)  ||  defined(__MSP430_HAS_PORT6_R__)
#define USE_PORT6
#endif

#if defined(__MSP430_HAS_PORT7__)  ||  defined(__MSP430_HAS_PORT7_R__)
#define USE_PORT7
#endif

#if defined(__MSP430_HAS_PORT8__)  ||  defined(__MSP430_HAS_PORT8_R__)
#define USE_PORT8
#endif

#if defined(__MSP430_HAS_PORT9__)  ||  defined(__MSP430_HAS_PORT9_R__)
#define USE_PORT9
#endif

#if defined(__MSP430_HAS_PORT10__)  ||  defined(__MSP430_HAS_PORT10_R__)
#define USE_PORT10
#endif

namespace IO
{

	//Port definitions for MSP430 families.

	#define MAKE_PORT(portName, dirName, pinName, selectName, interruptName, interruptEdge, resistorEnable, className, ID) \
		class className{\
		public:\
			typedef uint8_t DataT;\
			enum{DirBit = 1, AltSelBit = 2, ResEnBit = 4};\
		public:\
			enum PinConfiguration\
			{\
				AnalogIn = 0,\
				In = 0,\
				PullUpOrDownIn = ResEnBit,\
				Out = DirBit,\
				AltOut = DirBit | AltSelBit,\
			};\
			static void Write(DataT value)\
			{\
				portName = value;\
			}\
			static void ClearAndSet(DataT clearMask, DataT value)\
			{\
                portName &= ~clearMask;\
				portName |= value;\
			}\
			static void DirClearAndSet(DataT clearMask, DataT value)\
			{\
				dirName &= ~clearMask;\
				dirName |= value;\
			}\
			static DataT Read()\
			{\
				return portName;\
			}\
			static void DirWrite(DataT value)\
			{\
				dirName = value;\
			}\
			static DataT DirRead()\
			{\
				return dirName;\
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
			static void DirSet(DataT value)\
			{\
				dirName |= value;\
			}\
			static void DirClear(DataT value)\
			{\
				dirName &= ~value;\
			}\
			static void DirToggle(DataT value)\
			{\
				dirName ^= value;\
			}\
			static DataT PinRead()\
			{\
				return pinName;\
			}\
			template<unsigned pin, PinConfiguration configuration>\
			static void SetPinConfiguration()\
			{\
				BOOST_STATIC_ASSERT(pin < Width);\
				if(configuration & DirBit)\
					DirSet(1 << pin);\
				else\
					DirClear(1 << pin);\
				if(configuration & AltSelBit)\
					selectName |= (1 << pin);\
				else\
					selectName &= ~(1 << pin);\
				if(configuration & ResEnBit)\
					resistorEnable |= (1 << pin);\
				else\
					resistorEnable &= ~(1 << pin);\
			}\
			enum{Id = ID};\
			enum{Width=sizeof(DataT)*8};\
		};

	#ifdef USE_PORT0
	MAKE_PORT(P0OUT, P0DIR, P0IN, Port0, '0')
	#endif


	#ifdef USE_PORT1
	MAKE_PORT(P1OUT, P1DIR, P1IN, P1SEL, P1IE, P1IES, P1REN, Port1, '1')
	#endif


	#ifdef USE_PORT2
	MAKE_PORT(P2OUT, P2DIR, P2IN, P2SEL, P2IE, P2IES, P2REN, Port2, '2')
	#endif


	#ifdef USE_PORT3
	MAKE_PORT(P3OUT, P3DIR, P3IN, P3SEL, P3IE, P3IES, P3REN, Port3, '3')
	#endif


	#ifdef USE_PORT4
	MAKE_PORT(P4OUT, P4DIR, P4IN, P4SEL, P4IE, P4IES, P4REN, Port4, '4')
	#endif


	#ifdef USE_PORT5
	MAKE_PORT(P5OUT, P5DIR, P5IN, P5SEL, P5IE, P5IES, P5REN, Port5, '5')
	#endif

	#ifdef USE_PORT6
	MAKE_PORT(P6OUT, P6DIR, P6IN, P6SEL, P6IE, P6IES, P6REN, Port6, '6')
	#endif

	#ifdef USE_PORT7
	MAKE_PORT(P7OUT, P7DIR, P7IN, P7SEL, P7IE, P7IES, P7REN, Port7, '7')
	#endif

	#ifdef USE_PORT8
	MAKE_PORT(P8OUT, P8DIR, P8IN, P8SEL, P8IE, P8IES, P8REN, Port8, '8')
	#endif

	#ifdef USE_PORT9
	MAKE_PORT(P9OUT, P9DIR, P9IN, P9SEL, P9IE, P9IES, P9REN, Port9, '9')
	#endif

	#ifdef USE_PORT10
	MAKE_PORT(P10OUT, P10DIR, P10IN, P10SEL, P10IE, P10IES, P10REN, Port10, '10')
	#endif

}

#endif
