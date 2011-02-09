
#include <avr/io.h>
#include "iopins.h"
#include "pinlist.h"


using namespace IO;

typedef PinList<Pa4, Pa1, Pa6, Pa3>  Pins1;
typedef PinList<Pa2, Pa5, Pa0, Pa7>  Pins2;

class ListBase
{
	public:
	typedef uint8_t DataT;
	typedef NativePortBase::Configuration Configuration;
	virtual void Write(DataT value)=0;
	virtual void Set(DataT value)=0;
	virtual void Clear(DataT value)=0;
	virtual void SetConfiguration(Configuration value, DataT mask)=0;
	virtual DataT Read()=0;
	virtual DataT PinRead()=0;
};

template<class List>
class ListOfPins : public ListBase
{
	public:
	virtual void Write(DataT value)
	{
		List::Write(value);
	}
	virtual void Set(DataT value)
	{	
		List::Set(value);
	}
	virtual void Clear(DataT value)
	{
		List::Clear(value);
	}
	virtual void SetConfiguration(Configuration value, DataT mask)
	{
		List::SetConfiguration(value, mask);
	}
	virtual DataT Read()
	{
		return List::Read();
	}
	virtual DataT PinRead()
	{
		return List::PinRead();
	}
};


typedef void (* WriteFuncPtr)(uint8_t value);

class Bar
{
	public:
	Bar(WriteFuncPtr func)
	:_func(func)
	{}
	
	void Foo()
	{
		_func(0x55);
		_func(1);
		_func(2);
		_func(3);
	}

	WriteFuncPtr _func;
};

class B
{
	public:
	B(ListBase *list)
	:_list(list)
	{}
	
	void Foo()
	{
		_list->Write(0x55);	
	}

	ListBase *_list;
};

__attribute((OS_main))
int main()
{


	ListOfPins<Pins1> list1;
	ListOfPins<Pins2> list2;


	B b1(&list1);
	B b2(&list2);

	b1.Foo();
	b2.Foo();

	while(1)
	{}
}

extern "C" void __cxa_pure_virtual()
{

}
