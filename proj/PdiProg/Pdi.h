#pragma once
#include <util/delay.h>
#include "util.h"
#include "ProgInterface.h"
#include "timer.h"

namespace Pdi
{
	enum
	{
		CMD_LDS               = 0x00,
		CMD_LD                = 0x20,
		CMD_STS               = 0x40,
		CMD_ST                = 0x60,
		CMD_LDCS              = 0x80,
		CMD_REPEAT            = 0xA0,
		CMD_STCS              = 0xC0,
		CMD_KEY               = 0xE0,

		STATUS_REG            = 0x0,
		RESET_REG             = 0x1,
		CTRL_REG              = 0x2,

		STATUS_NVM            = 0x02,
		RESET_KEY             = 0x59,

		DATSIZE_1BYTE         = 0x0,
		DATSIZE_2BYTES        = 0x1,
		DATSIZE_3BYTES        = 0x2,
		DATSIZE_4BYTES        = 0x3,

		POINTER_INDIRECT      = 0x0,
		POINTER_INDIRECT_PI   = 0x1,
		POINTER_DIRECT        = 0x2
	};

	struct PdiSoftwareData
	{
		uint16_t data;
		uint8_t bitsCount;
		bool isSending;
	};

	template<class DataPin, class ClockPin, class Timer=Timer0>
	class PdiSoftwarePhisical :public ProgInterface
	{
		public:
		enum {FrameLength = 12};

		void Enable()
		{
			ClockPin::SetDirWrite();
			DataPin::SetDirWrite();
			DataPin::Set();
			_delay_us(1);
			Timer::Start(Timer::Ck8);
			Timer::EnableOverflowInterrupt();
			Break();
			Break();
		}

		void Disable()
		{
			Timer::Stop();
			DataPin::Clear();
			DataPin::SetDirRead();
			ClockPin::SetDirRead();
		}
	
		void WriteByte(uint8_t c)
		{
			if(!(_data.isSending))
			{
				DataPin::Set();
				DataPin::SetDirWrite();
				_data.isSending = true;
			}

			uint16_t value = (0x0e << 9) | (uint16_t(c) << 1) | (0 << 0);

			uint8_t parity    = c;
			while (parity)
			{
				value ^= (1 << 9);
				parity   &= (parity - 1);
			}

			while (_data.bitsCount);

			_data.data = value;
			_data.bitsCount = FrameLength;
		}

		uint8_t ReadByte()
		{
			if (_data.isSending)
			{
				while (_data.bitsCount);
				DataPin::SetDirRead();
				DataPin::Clear();
				_data.isSending = false;
			}

			_data.bitsCount = FrameLength;
			while (_data.bitsCount);
	
			return (uint8_t)_data.data;
		}

		void Reset()
		{
			DataPin::Clear();
			DataPin::SetDirRead();
			ClockPin::SetDirWrite();
			ClockPin::Set();
			_delay_ms(10);
			ClockPin::Clear();
			_delay_us(10);
			ClockPin::Set();
		}

		void Break()
		{
			if (!(_data.isSending))
			{
				DataPin::Set();
				DataPin::SetDirWrite();
				_data.isSending = true;
			}

			while (_data.bitsCount);

			_data.data     = 0x0FFF;
			_data.bitsCount = FrameLength;
		}

		static void TimerHandler()
		{
			Timer::Set(255-20);

			ClockPin::Togle();

			if(!(_data.bitsCount))
			  return;

			if(ClockPin::IsSet())
			{
				if(_data.isSending)
				  return;
		  
				if((_data.bitsCount == FrameLength) && DataPin::IsSet())
				  return;

				if(DataPin::IsSet())
					_data.data |= (1 << (FrameLength - 1));

				_data.data >>= 1;
				_data.bitsCount--;
			}
			else
			{
				if (!_data.isSending)
				  return;

				DataPin::Set(_data.data & 0x01);

				_data.data >>= 1;
				_data.bitsCount--;
			}
		}

	protected:
		static volatile PdiSoftwareData _data;
	};

	template<class DataPin, class ClockPin, class Timer>
	volatile PdiSoftwareData PdiSoftwarePhisical<DataPin, ClockPin, Timer>::_data;

}
