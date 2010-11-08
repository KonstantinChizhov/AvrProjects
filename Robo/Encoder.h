#pragma once
#include <static_assert.h>

template<class DataT, class Pins>
class Encoder
{
	BOOST_STATIC_ASSERT((Pins::Length & 1) == 0);
public:
	enum {Channels = Pins::Length / 2};
	typedef typename Pins::DataType PortType;

	static void CaptureHandler()
	{
		PortType current = Pins::PinRead();
	}

	template<unsigned number>
	static DataT Value()
	{
		return 0;
	}

	static DataT _values[Channels];
	static PortType _prevValue;
};

