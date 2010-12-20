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
#include "MSP430/Msp430Ports.h"
#elif defined(__ICCAVR__) || defined(__AVR__)

#if defined(PORTCFG_MPCMASK) //XMega family
#include "AVR/XMegaPorts.h"
#else
#include "AVR/AvrPorts.h"
#endif

#elif defined(__ICCARM__) || defined ( __CC_ARM   ) //IAR Systems for ARM
#include "ARM/Stm32Ports.h"
#else

#warning "Not supported arch. Test mode only."
#include "TestPort.h"
#endif

namespace IO
{
	class NullPort
	{
 	public:
		enum PinConfiguration\
		{\
			AnalogIn = 0,
			In = 0x00,
			PullUpOrDownIn = 0x00,
			Out = 0x01,
			AltOut = 0x01,
		};
			  
		typedef uint8_t DataT;
		static void Write(DataT value)
		{	}
		static void ClearAndSet(DataT clearMask, DataT value)
		{	}
		static void DirClearAndSet(DataT clearMask, DataT value)
		{	}
		static DataT Read()
		{
			return 0;
		}
		static void DirWrite(DataT value)
		{	}
		static DataT DirRead()
		{
			return 0;
		}
		static void Set(DataT value)
		{	}
		static void Clear(DataT value)
		{	}
		static void Togle(DataT value)
		{	}
		static void DirSet(DataT value)
		{	}
		static void DirClear(DataT value)
		{	}
		static void DirTogle(DataT value)
		{	}
		static DataT PinRead()
		{
			return 0;
		}
		
		template<unsigned pin, PinConfiguration configuration>
		static void SetPinConfiguration()
		{
			BOOST_STATIC_ASSERT(pin < Width);
		}
		
		enum{Id = '-'};
		enum{Width=sizeof(DataT)*8};
	};
	
	typedef TPin<NullPort, 0> NullPin;
}

#endif//IOPORTS_HPP
