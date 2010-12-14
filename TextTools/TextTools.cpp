#include <avr/io.h>
#include <avr/pgmspace.h>


template<class T>
class ProgmemPointer
{
public:
	ProgmemPointer(T *address)
		:_address((size_t)address)
	{
	}

	ProgmemPointer& operator ++()
	{
		_address++;
		return *this;
	}

	ProgmemPointer& operator ++(int)
	{
		ProgmemPointer tmp = *this;
		_address++;
		return tmp;
	}

	ProgmemPointer& operator --()
	{
		_address--;
		return *this;
	}

	ProgmemPointer& operator --(int)
	{
		ProgmemPointer tmp = *this;
		_address--;
		return tmp;
	}

	inline T operator *()
	{
		union 
		{
			T value;
			uint8_t bytes[sizeof(T)];
		};
		for(unsigned i = 0; i<sizeof(T); ++i)
			bytes[i] = pgm_read_byte(_address + i);
		return value;
	}


private:
	size_t _address;
};



class Text
{
public:
	Text(const char *text, unsigned lenght)
	:_text(text),
	_lenght(lenght)
	{}
	
//	operator const char * ()const
//	{return _text;}

	char operator [](unsigned index) const
	{
		return pgm_read_byte(_text + index);
	}

	unsigned Size() const
	{
		return _lenght;
	}

private:
	const char *const _text;
	const unsigned _lenght;
};


void DoSomethisng(Text & text)
{
}

void DoSomethisng(const char * s)
{
	while(*s)
	{
		PORTA = *s++;
	}
}

int main()
{
	//Text text(PSTR("12345"), 5);

	//DoSomethisng(PSTR("12345"));

ProgmemPointer<char> ptr(PSTR("12345"));

PORTA = *++ptr;
PORTA = *++ptr;
PORTA = *++ptr;
PORTA = *++ptr;
	while(1);
}
