#pragma once


template<class O1, class O2>
class SimpleDriver
{
	public:
	static void Forward()
	{
		O1::SetConfiguration(O1::Port::Out);
		O2::SetConfiguration(O2::Port::Out);
		O1::Set();
		O2::Clear();
	}

	static void Back()
	{
		O1::SetConfiguration(O1::Port::Out);
		O2::SetConfiguration(O2::Port::Out);
		O2::Set();
		O1::Clear();
	}

	static void Break()
	{
		O1::SetConfiguration(O1::Port::Out);
		O2::SetConfiguration(O2::Port::Out);
		O1::Clear();
		O2::Clear();
	}
};
