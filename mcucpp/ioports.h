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
#define IOPORTS_HPP

#include "gpiobase.h"

#if defined(__ICC430__) || defined(__MSP430__)
#include "MSP430/Msp430Ports.h"
#elif defined(__ICCAVR__) || defined(__AVR__)

#if defined(PORTCFG_MPCMASK) //XMega family
#include "AVR/XMegaPorts.h"
#else
#include "AVR/AvrPorts.h"
#endif

#elif defined(__ICCARM__) || defined ( __CC_ARM   ) || defined(__arm__)
#include "ARM/Stm32Ports.h"
#else

#warning "Not supported arch. Test mode only."
#include "TestPort.h"
#endif

namespace IO
{	
	class NullPort :public GpioBase
	{
 	public:
		typedef DontCareConfiguration Configuration;
		typedef GpioBase Base;
		typedef uint8_t DataT;
		static void Write(DataT value)
		{	}
		static void ClearAndSet(DataT clearMask, DataT value)
		{	}
		static DataT Read()
		{
			return 0;
		}
		static void Set(DataT value)
		{	}
		static void Clear(DataT value)
		{	}
		static void Togle(DataT value)
		{	}
		static DataT PinRead()
		{
			return 0;
		}

		template<unsigned pin, class Configuration>
		static void SetPinConfiguration(Configuration configuration)
		{
			//Nothing to do
		}
		
		template<class Configuration>
		static void SetConfiguration(DataT mask, Configuration configuration)
		{
			//Nothing to do
		}
		enum{Id = '-'};
		enum{Width=sizeof(DataT)*8};
	};

	typedef TPin<NullPort, 0> NullPin;
}

#endif//IOPORTS_HPP
