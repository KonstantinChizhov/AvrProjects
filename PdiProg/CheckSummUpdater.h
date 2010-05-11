
#include "Crc16.h"

	template<class DATA_SOURCE, bool calcRxCrc=false>
	class CheckSummUpdater :public DATA_SOURCE
	{
	public:
		static void Putch(uint8_t c)
		{
			DATA_SOURCE::Putch(c);
			_writeCrc = Crc16_0x8408(c, _writeCrc);
		}

		static void Getch(uint8_t &c)
		{
			DATA_SOURCE::Getch(c);
			if(calcRxCrc)
				_readCrc = Crc16_0x8408(c, _readCrc);
		}

		static void ResetWriteCrc()
		{
			_writeCrc = 0xffff;
		}

		static void WriteCrc()
		{
			DATA_SOURCE::Putch(_writeCrc & 0xff);
			DATA_SOURCE::Putch((_writeCrc >> 8) & 0xff);
		}

		static void ResetReadCrc()
		{
			_readCrc = 0xffff;
		}

		static uint16_t GetReciveCrc()
		{
			return _readCrc;
		}

		static uint16_t _writeCrc;
		static uint16_t _readCrc;
	};

	template<class DATA_SOURCE, bool calcRxCrc>
	uint16_t CheckSummUpdater<DATA_SOURCE, calcRxCrc>::_writeCrc=0xffff;

	template<class DATA_SOURCE, bool calcRxCrc>
	uint16_t CheckSummUpdater<DATA_SOURCE, calcRxCrc>::_readCrc=0xffff;
