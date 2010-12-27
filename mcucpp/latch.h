#pragma once
#ifndef LATCH_H
#define LATCH_H

#include "gpiobase.h"

//serial-in, parallel-out shift register with output latches, somthing like 74HC595
	class LatchBase : public IO::GpioBase
	{
		public:
		//typedef IO::GpioBase Base;
		typedef LatchBase Base;
		//SetConfiguration is do nothing function 
		// and accepts any configuration type
		typedef DontCareConfiguration Configuration;
		//enum Configuration{Out};
		static Configuration MapConfiguration(GenericConfiguration config)
		{
			return Configuration();
		}
	};
	
	template<class ClockPin, class DataPin, class LatchPin, unsigned ID, class T = uint8_t>
	class ThreePinLatch :public LatchBase
	{
		public:
		
		typedef T DataT;
		enum{Id = ID};
		enum{Width=sizeof(DataT)*8};

		static void Write(T value)
		{	
			_currentValue = value;
			for(uint8_t i=0; i < Width; ++i)
			{
				DataPin::Set(value & 1);
				ClockPin::Set();
				value >>= 1;
				ClockPin::Clear();
			}
			LatchPin::Set();
			LatchPin::Clear();
		}
		static DataT Read()
		{
				return _currentValue;
		}
		static void ClearAndSet(DataT clearMask, DataT value)
		{
			Write(_currentValue = (_currentValue & ~clearMask) | value);
		}
	
		static void Set(DataT value)
		{
			Write(_currentValue |= value);
		}
		static void Clear(DataT value)
		{
			Write(_currentValue &= ~value);
		}
		static void Togle(DataT value)
		{
			Write(_currentValue ^= value);
		}

		static DataT DirRead()
		{
			return 0xff;
		}

		static void DirClearAndSet(DataT clearMask, DataT value)
		{	}

		static void DirWrite(DataT value)
		{	}

		static void DirSet(DataT value)
		{	}

		static void DirClear(DataT value)
		{	}

		static void DirTogle(DataT value)
		{	}
		
		template<unsigned pin, class ConfigurationT>
		static void SetPinConfiguration(ConfigurationT configuration)
		{
			BOOST_STATIC_ASSERT(pin < Width);
			//Nothing to do
		}
		
		template<class ConfigurationT>
		static void SetConfiguration(DataT mask, ConfigurationT configuration)
		{
			//Nothing to do
		}
		protected:
		static DataT _currentValue;
	};

	template<class ClockPin, class DataPin, class LatchPin, unsigned ID, class T>
	T ThreePinLatch<ClockPin, DataPin, LatchPin, ID, T>::_currentValue = 0;

#endif
