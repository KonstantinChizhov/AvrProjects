
//*****************************************************************************
//
// Title		: C++ IO for ATMEL AVR
// Author		: Konstantin Chizhov
// Date			: 2010
// Target MCU	: Atmel MEGA, TINY, XMega AVR, TI Msp430
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

#include "iopin.h"
#include "ioports.h"

#define DECLARE_PORT_PINS(PORT_TYPE_NAME, PIN_NAME_PREFIX) \
	typedef TPin<PORT_TYPE_NAME, 0> PIN_NAME_PREFIX ## 0;\
	typedef TPin<PORT_TYPE_NAME, 1> PIN_NAME_PREFIX ## 1;\
	typedef TPin<PORT_TYPE_NAME, 2> PIN_NAME_PREFIX ## 2;\
	typedef TPin<PORT_TYPE_NAME, 3> PIN_NAME_PREFIX ## 3;\
	typedef TPin<PORT_TYPE_NAME, 4> PIN_NAME_PREFIX ## 4;\
	typedef TPin<PORT_TYPE_NAME, 5> PIN_NAME_PREFIX ## 5;\
	typedef TPin<PORT_TYPE_NAME, 6> PIN_NAME_PREFIX ## 6;\
	typedef TPin<PORT_TYPE_NAME, 7> PIN_NAME_PREFIX ## 7;\
	typedef TPin<PORT_TYPE_NAME, 8> PIN_NAME_PREFIX ## 8;\
	typedef TPin<PORT_TYPE_NAME, 9> PIN_NAME_PREFIX ## 9;\
	typedef TPin<PORT_TYPE_NAME, 10> PIN_NAME_PREFIX ## 10;\
	typedef TPin<PORT_TYPE_NAME, 11> PIN_NAME_PREFIX ## 11;\
	typedef TPin<PORT_TYPE_NAME, 12> PIN_NAME_PREFIX ## 12;\
	typedef TPin<PORT_TYPE_NAME, 13> PIN_NAME_PREFIX ## 13;\
	typedef TPin<PORT_TYPE_NAME, 14> PIN_NAME_PREFIX ## 14;\
	typedef TPin<PORT_TYPE_NAME, 15> PIN_NAME_PREFIX ## 15;\
	typedef TPin<PORT_TYPE_NAME, 16> PIN_NAME_PREFIX ## 16;\
	typedef TPin<PORT_TYPE_NAME, 17> PIN_NAME_PREFIX ## 17;\
	typedef TPin<PORT_TYPE_NAME, 18> PIN_NAME_PREFIX ## 18;\
	typedef TPin<PORT_TYPE_NAME, 19> PIN_NAME_PREFIX ## 19;\
	typedef TPin<PORT_TYPE_NAME, 20> PIN_NAME_PREFIX ## 20;\
	typedef TPin<PORT_TYPE_NAME, 21> PIN_NAME_PREFIX ## 21;\
	typedef TPin<PORT_TYPE_NAME, 22> PIN_NAME_PREFIX ## 22;\
	typedef TPin<PORT_TYPE_NAME, 23> PIN_NAME_PREFIX ## 23;\
	typedef TPin<PORT_TYPE_NAME, 24> PIN_NAME_PREFIX ## 24;\
	typedef TPin<PORT_TYPE_NAME, 25> PIN_NAME_PREFIX ## 25;\
	typedef TPin<PORT_TYPE_NAME, 26> PIN_NAME_PREFIX ## 26;\
	typedef TPin<PORT_TYPE_NAME, 27> PIN_NAME_PREFIX ## 27;\
	typedef TPin<PORT_TYPE_NAME, 28> PIN_NAME_PREFIX ## 28;\
	typedef TPin<PORT_TYPE_NAME, 29> PIN_NAME_PREFIX ## 29;\
	typedef TPin<PORT_TYPE_NAME, 30> PIN_NAME_PREFIX ## 30;\
	typedef TPin<PORT_TYPE_NAME, 31> PIN_NAME_PREFIX ## 31;


namespace IO
{
	//Short pin definations

	#ifdef USE_PORTA
	DECLARE_PORT_PINS(Porta, Pa)
	#endif

	#ifdef USE_PORTB
	DECLARE_PORT_PINS(Portb, Pb)
	#endif

	#ifdef USE_PORTC
	DECLARE_PORT_PINS(Portc, Pc)
	#endif

	#ifdef USE_PORTD
	DECLARE_PORT_PINS(Portd, Pd)
	#endif

	#ifdef USE_PORTE
	DECLARE_PORT_PINS(Porte, Pe)
	#endif

	#ifdef USE_PORTF
	DECLARE_PORT_PINS(Portf, Pf)
	#endif

	#ifdef USE_PORTG
	DECLARE_PORT_PINS(Portg, Pg)
	#endif

	#ifdef USE_PORTH
	DECLARE_PORT_PINS(Porth, Ph)
	#endif

	#ifdef USE_PORTJ
	DECLARE_PORT_PINS(Portj, Pj)
	#endif

	#ifdef USE_PORTK
	DECLARE_PORT_PINS(Portk, Pk)
	#endif

	#ifdef USE_PORTQ
	DECLARE_PORT_PINS(Portq, Pq)
	#endif

	#ifdef USE_PORTR
	DECLARE_PORT_PINS(Portr, Pr)
	#endif

	#ifdef USE_PORT0
	DECLARE_PORT_PINS(Port0, P0_)
	#endif

	#ifdef USE_PORT1
	DECLARE_PORT_PINS(Port1, P2_)
	#endif

	#ifdef USE_PORT2
	DECLARE_PORT_PINS(Port2, P4_)
	#endif

	#ifdef USE_PORT3
	DECLARE_PORT_PINS(Port3, P3_)
	#endif

	#ifdef USE_PORT4
	DECLARE_PORT_PINS(Port4, P4_)
	#endif

	#ifdef USE_PORT5
	DECLARE_PORT_PINS(Port5, P5_)
	#endif

	#ifdef USE_PORT6
	DECLARE_PORT_PINS(Port6, P6_)
	#endif

	#ifdef USE_PORT7
	DECLARE_PORT_PINS(Port7, P7_)
	#endif

	#ifdef USE_PORT8
	DECLARE_PORT_PINS(Port8, P8_)
	#endif

	#ifdef USE_PORT9
	DECLARE_PORT_PINS(Port9, P9_)
	#endif

	#ifdef USE_PORT10
	DECLARE_PORT_PINS(Port10, P10_)
	#endif

}
