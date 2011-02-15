
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
	void Write(DataT value){_Write(value);}
	void Set(DataT value){_Set(value);}
	void Clear(DataT value){_Clear(value);}
	void SetConfiguration(Configuration value, DataT mask){_SetConfiguration(value, mask);}
	DataT PinRead(){return _PinRead();}
	private:
	virtual void _Write(DataT value)=0;
	virtual void _Set(DataT value)=0;
	virtual void _Clear(DataT value)=0;
	virtual void _SetConfiguration(Configuration value, DataT mask)=0;
	virtual DataT _PinRead()=0;
};

template<class List>
class ListOfPins : public ListBase
{
	private:
	virtual void _Write(DataT value)
	{
		List::Write(value);
	}
	virtual void _Set(DataT value)
	{	
		List::Set(value);
	}
	virtual void _Clear(DataT value)
	{
		List::Clear(value);
	}
	virtual void _SetConfiguration(Configuration value, DataT mask)
	{
		List::SetConfiguration(value, mask);
	}

	virtual DataT _PinRead()
	{
		return List::PinRead();
	}
};


typedef void (* WriteFuncPtr)(uint8_t value);
typedef void (* ConfigFunc)(NativePortBase::Configuration config, uint8_t mask);
typedef uint8_t (* ReadFuncPtr)();

struct Funcs
{
	WriteFuncPtr Write;
	WriteFuncPtr Set;
	WriteFuncPtr Clear;
	ConfigFunc Config;
	ReadFuncPtr Read;
};

class Bar
{
	public:
	Bar(Funcs &funcs)
	:_funcs(funcs)
	{}
	
	void Foo()
	{
		_funcs.Config(NativePortBase::Out, 0xff);
		_funcs.Write(123);
		_funcs.Set(0x55);
		_funcs.Clear(0xff);
		_funcs.Write(_funcs.Read()+1);
	}

	Funcs &_funcs;
};

class B
{
	public:
	B(ListBase &list)
	:_list(list)
	{}
	
	void Foo()
	{
		_list.SetConfiguration(NativePortBase::Out, 0xff);
		_list.Write(123);
		_list.Set(0x55);
		_list.Clear(0xff);
		_list.Write(_list.PinRead()+1);
	}

	ListBase &_list;
};

	Funcs f = 
	{
		Pins1::Write,
		Pins1::Set,
		Pins1::Clear,
		Pins1::SetConfiguration,
		Pins1::PinRead
	};

	Funcs f2 = 
	{
		Pins2::Write,
		Pins2::Set,
		Pins2::Clear,
		Pins2::SetConfiguration,
		Pins2::PinRead
	};


__attribute((OS_main))
int main()
{

#if 0

	Bar bar(f);
	Bar bar2(f2);
	bar.Foo();
	bar2.Foo();
#else
	ListOfPins<Pins1> list1;
	ListOfPins<Pins2> list2;


	B b1(list1);
	b1.Foo();
	B b2(list2);
	b2.Foo();

#endif
	while(1)
	{}
}

extern "C" void __cxa_pure_virtual()
{

}
