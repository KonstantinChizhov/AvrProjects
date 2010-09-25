//*****************************************************************************
//
// Title		: C++ IO for MCU
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

#ifndef IOPORTS_HPP
#define IOPORTS_HPP


#if defined(__ICC430__) || defined(__MSP430__)
#include "Msp430Ports.h"
#elif defined(__ICCAVR__) || defined(__AVR__)

#if defined(PORTCFG_MPCMASK) //XMega family
#include "XMegaPorts.h"
#else
#include "AvrPorts.h"
#endif

#else
#error "Not supported arch."

#endif


#endif//IOPORTS_HPP
