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
		if(	_prevState != current)
			return;
		int decrement = -1;

		switch(_prevState)
		{
		case 0:
			if(current == 2) decrement = 1;
			break;
		case 1:
			if(current == 0) decrement = 1;
			break;
		case 2:
			if(current == 3) decrement = 1;
			break; 
		case 3:
			if(current == 1) decrement = 1;
			break;
		}

		_value -= decrement;
		_prevState = current;
	}

	static DataT Value()
	{
		return _value;
	}

	static DataT _value;
	static PortType _prevState;
};

	template<class DataT, class Pins>
	DataT Encoder<DataT, Pins>::_value;
	template<class DataT, class Pins>
	typename Encoder<DataT, Pins>::PortType Encoder<DataT, Pins>::_prevState;
