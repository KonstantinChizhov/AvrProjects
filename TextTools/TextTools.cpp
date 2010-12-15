#include <avr/io.h>
#include <avr/pgmspace.h>


template<class T, class PtrType = T*>
class ProgmemPtr
{
	typedef ProgmemPtr Self;
public:
	ProgmemPtr(T *address)
		:_address(address)
	{
	}

	Self& operator ++()
	{
		_address++;
		return *this;
	}

	Self& operator ++(int)
	{
		Self tmp = *this;
		_address++;
		return tmp;
	}

	Self& operator --()
	{
		_address--;
		return *this;
	}

	Self& operator --(int)
	{
		Self tmp = *this;
		_address--;
		return tmp;
	}

	Self& operator +=(int value)
	{
		_address += value;
		return *this;
	}

	Self& operator -=(int value)
	{
		_address += value;
		return *this;
	}

	bool operator !=(const Self &other) const
	{
		return _address != other._address;
	}

	bool operator ==(const Self &other) const
	{
		return _address == other._address;
	}

	inline const T operator *()const
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
	PtrType _address;
};

template<class T>
void Put(T s)
{
  	while(*s)
		PORTA = *s++; 
}

#define PROG(TYPE, NAME, INITIALIZER)\
		PROGMEM static TYPE NAME ## _storage[] = INITIALIZER; \
		ProgmemPtr<TYPE> NAME(NAME ## _storage)



int main()
{


PROG(char, str2, "1234567789");

	Put(str2);
	
	while(1);
}
