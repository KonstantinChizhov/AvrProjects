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

#include "static_assert.h"
#include <stdint.h>
namespace IO
{

	// this class represents one pin in a IO port.
	// It is fully static.
	template<class PORT, uint8_t PIN, class CONFIG_PORT = PORT>
	class TPin
	{
		BOOST_STATIC_ASSERT(PIN < PORT::Width);
	public:
		typedef PORT Port;
		typedef CONFIG_PORT ConfigPort;
		typedef typename ConfigPort::Configuration Configuration;

		static const unsigned Number = PIN;
		static const bool Inverted = false;

		static void Set()
		{
			Set(true);
		}

		static void Set(bool val)
		{
			if(val)
				PORT::template Set<1 << PIN>();
			else
				PORT::template Clear<1 << PIN>();
		}

		static void SetDir(uint8_t val)
		{
			if(val)
				SetDirWrite();
			else SetDirRead();
		}

		static void Clear()
		{
			Set(false);
		}

		static void Toggle()
		{
			PORT::Toggle(1 << PIN);
		}

		static void SetDirRead()
		{
			PORT::template SetPinConfiguration<PIN>( PORT::In);
		}

		static void SetDirWrite()
		{
			ConfigPort:: template SetPinConfiguration<PIN>(PORT::Out);
		}
		
		static void SetConfiguration(Configuration configuration)
		{
			ConfigPort:: template SetPinConfiguration<PIN>(configuration);
		}
		
		template<Configuration configuration>
		static void SetConfiguration()
		{
			ConfigPort:: template SetConfiguration<1 << PIN, configuration>();
		}

		static uint8_t IsSet()
		{
			return PORT::PinRead() & (uint8_t)(1 << PIN);
		}

		static void WaiteForSet()
		{
			while(IsSet()==0){}
		}

		static void WaiteForClear()
		{
			while(IsSet()){}
		}
	};

	template<class PORT, uint8_t PIN, class CONFIG_PORT = PORT>
	class InvertedPin :public TPin<PORT, PIN, CONFIG_PORT>
	{
	public:
		static const bool Inverted = true;

		static void Set(bool val)
		{
			if(val)
				PORT::template Clear<1 << PIN>();
			else
				PORT::template Set<1 << PIN>();
		}

		static void Set()
		{
			Set(true);
		}

		static void Clear()
		{
			Set(false);
		}
	};
}
