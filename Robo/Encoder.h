#pragma once
#include <static_assert.h>

template<class DataT, class Pins>
class Encoder
{
	BOOST_STATIC_ASSERT(Pins::Length == 2);
public:
	typedef typename Pins::DataType PortType;

	static void CaptureHandler()
	{
		PortType current = Pins::PinRead();
		
		uint8_t tmp = _prevState | current << 2;

		switch(tmp)
		{
			case 0x04: case 0x02: case 0x09: case 0x0D: case 0x0B:
				_value ++;
			break;
			case 0x08: case 0x06: case 0x0E: case 0x01: case 0x07:
				_value --;
			break;
		}
		_prevState = current;
	}

	static DataT Value()
	{
		return _value;
	}

	static volatile DataT _value;
	static volatile PortType _prevState;
};

	template<class DataT, class Pins>
	volatile DataT Encoder<DataT, Pins>::_value;
	template<class DataT, class Pins>
	volatile typename Encoder<DataT, Pins>::PortType Encoder<DataT, Pins>::_prevState;
