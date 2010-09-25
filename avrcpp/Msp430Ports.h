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

	#define MAKE_PORT(portName, dirName, pinName, className, ID) \
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
				dirName = (dirName & ~clearMask) | value;\
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
			static void Togle(DataT value)\
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
			static void DirTogle(DataT value)\
			{\
				dirName ^= value;\
			}\
			static DataT PinRead()\
			{\
				return pinName;\
			}\
			enum{Id = ID};\
			enum{Width=sizeof(DataT)*8};\
		};

	#ifdef USE_PORT0
	MAKE_PORT(P0OUT, P0DIR, P0IN, Port0, '0')
	#endif


	#ifdef USE_PORT1
	MAKE_PORT(P1OUT, P1DIR, P1IN, Port1, '1')
	#endif


	#ifdef USE_PORT2
	MAKE_PORT(P2OUT, P2DIR, P2IN, Port2, '2')
	#endif


	#ifdef USE_PORT3
	MAKE_PORT(P3OUT, P3DIR, P3IN, Port3, '3')
	#endif


	#ifdef USE_PORT4
	MAKE_PORT(P4OUT, P4DIR, P4IN, Port4, '4')
	#endif


	#ifdef USE_PORT5
	MAKE_PORT(P5OUT, P5DIR, P5IN, Port5, '5')
	#endif

	#ifdef USE_PORT6
	MAKE_PORT(P6OUT, P6DIR, P6IN, Port6, '6')
	#endif

	#ifdef USE_PORT7
	MAKE_PORT(P7OUT, P7DIR, P7IN, Port7, '7')
	#endif

	#ifdef USE_PORT8
	MAKE_PORT(P8OUT, P8DIR, P8IN, Port8, '8')
	#endif

	#ifdef USE_PORT9
	MAKE_PORT(P9OUT, P9DIR, P9IN, Port9, '9')
	#endif

	#ifdef USE_PORT10
	MAKE_PORT(P10OUT, P10DIR, P10IN, Port10, '10')
	#endif

}

#endif
