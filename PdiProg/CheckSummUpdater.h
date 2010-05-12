
#include "Crc16.h"

	template<class DATA_SOURCE, bool calcRxCrc=false>
	class CheckSummUpdater :public DATA_SOURCE
	{
	public:
		using DATA_SOURCE::Read;
		using DATA_SOURCE::Write;

		static uint8_t Putch(uint8_t c)
		{
			if(DATA_SOURCE::Putch(c))
			{
				_writeCrc = Crc16_0x8408(c, _writeCrc);
				return 1;
			}
			return 0;
		}

		static uint8_t Getch(uint8_t &c)
		{
			if(DATA_SOURCE::Getch(c))
			{
				if(calcRxCrc)
					_readCrc = Crc16_0x8408(c, _readCrc);
				return 1;
			}
			return 0;
		}

		static void ResetWriteCrc()
		{
			_writeCrc = 0xffff;
		}

		static void WriteCrc()
		{
			DATA_SOURCE::Write(uint8_t(_writeCrc & 0xff));
			DATA_SOURCE::Write(uint8_t((_writeCrc >> 8) & 0xff));
		}

		static void ResetReadCrc()
		{
			_readCrc = 0xffff;
		}

		static uint16_t GetReciveCrc()
		{
			return _readCrc;
		}

		static void BeginTxFrame()
		{
			ResetWriteCrc();
			DATA_SOURCE::BeginTxFrame();
		}

		static void EndTxFrame()
		{
			WriteCrc();
			DATA_SOURCE::EndTxFrame();
		}

		static void BeginRx()
		{
			DATA_SOURCE::BeginTxFrame();
		}

		static void EndRx()
		{
			DATA_SOURCE::BeginTxFrame();
		}

		static uint16_t _writeCrc;
		static uint16_t _readCrc;
	};

	template<class DATA_SOURCE, bool calcRxCrc>
	uint16_t CheckSummUpdater<DATA_SOURCE, calcRxCrc>::_writeCrc=0xffff;

	template<class DATA_SOURCE, bool calcRxCrc>
	uint16_t CheckSummUpdater<DATA_SOURCE, calcRxCrc>::_readCrc=0xffff;
