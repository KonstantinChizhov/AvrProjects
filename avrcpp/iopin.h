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

		static void Togle()
		{
			PORT::Togle(1 << PIN);
		}

		static void SetDirRead()
		{
			PORT::DirClear(1 << PIN);
		}

		static void SetDirWrite()
		{
			PORT::DirSet(1 << PIN);
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
