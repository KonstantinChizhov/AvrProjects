//*****************************************************************************
//
// Title		: C++ IO for ATMEL AVR
// Author		: Konstantin Chizhov
// Date			: 2010
// Target MCU	: paltform indepandent
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

#include "static_assert.h"
#include <stdint.h>
namespace IO
{

	// this class represents one pin in a IO port.
	// It is fully static.
	template<class PORT, uint8_t PIN>
	class TPin
	{
		BOOST_STATIC_ASSERT(PIN < PORT::Width);
	public:
		typedef PORT Port;
		typedef typename PORT::Configuration Configuration;
		enum{Number = PIN};

		static void Set()
		{
			PORT::Set(1 << PIN);
		}

		static void Set(uint8_t val)
		{
			if(val)
				Set();
			else Clear();
		}

		static void SetDir(uint8_t val)
		{
			if(val)
				SetDirWrite();
			else SetDirRead();
		}

		static void Clear()
		{
			PORT::Clear(1 << PIN);
		}

		static void Toggle()
		{
			PORT::Toggle(1 << PIN);
		}

		static void SetDirRead()
		{
			PORT::template SetPinConfiguration<PIN, PORT::In>();
		}

		static void SetDirWrite()
		{
			PORT:: template SetPinConfiguration<PIN, PORT::Out>();
		}
		
		template<Configuration configuration>
		static void SetConfiguration()
		{
			PORT:: template SetPinConfiguration<PIN, configuration>();
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
}
