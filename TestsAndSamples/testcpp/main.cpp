
#include <avr/io.h>
#include "iopins.h"
#include "pinlist.h"
#include <avr/pgmspace.h>
#include <avr/eeprom.h>

using namespace IO;


template<class T, class Accessor>
class BasePtr
{
	typedef BasePtr Self;
public:
	BasePtr(T *address=0)
		:_address(address)
	{
	}

	inline Self& operator ++()
	{
		_address++;
		return *this;
	}

	inline Self operator ++(int)
	{
		Self tmp = *this;
		_address++;
		return tmp;
	}

	inline Self& operator --()
	{
		_address--;
		return *this;
	}

	inline Self operator --(int)
	{
		Self tmp = *this;
		_address--;
		return tmp;
	}

	inline Self& operator +=(int value)
	{
		_address += value;
		return *this;
	}

	inline Self& operator -=(int value)
	{
		_address -= value;
		return *this;
	}

	inline Self operator +(int value)
	{
		return Self(_address + value);
	}

	inline Self operator -(int value)
	{
		return Self(_address - value);
	}

	inline bool operator !=(const Self &other) const
	{
		return _address != other._address;
	}

	inline bool operator ==(const Self &other) const
	{
		return _address == other._address;
	}

	inline const T operator *()const
	{
		union 
		{
			T value;
			uint8_t bytes[sizeof(T)];
		}data;

		for(unsigned i = 0; i<sizeof(T); ++i)
			data.bytes[i] = Accessor::Read(_address + i);
		return data.value;
	}

private:
	T * _address;
};


template<class T>
class FlashPtr :public BasePtr<T, FlashPtr<T> >
{
	public:
	FlashPtr(T *address)
	:BasePtr<T, FlashPtr<T> >(address)
	{}

	static uint8_t Read(T *addr)
	{
		return pgm_read_byte((const uint8_t*)addr);
	}
};

template<class T>
class EepromPtr :public BasePtr<T, EepromPtr<T> >
{
	public:
	EepromPtr(T *address)
	:BasePtr<T, EepromPtr<T> >(address)
	{}

	static uint8_t Read(T *addr)
	{
		return eeprom_read_byte((const uint8_t*)addr);
	}
};



static inline void UsartWriteByte(char value)
{
	while(!(UCSRA & (1 << UDRE)));
	UDR = value;
}

template<class T>
void UsartPuts(T str)
{
  	char c;
 	while((c = *str++))
	{
  		UsartWriteByte(c);
	}
}

char strInRam[] = "Hello from RAM";
PROGMEM char strInFlash[] = "Hello from flash"; 
EEMEM char strInEeprom[] = "Hello from EEPROM";



__attribute((OS_main))
int main()
{
	const FlashPtr<char> ptrInFlash(strInFlash);
	const EepromPtr<char> ptrInEeprom(strInEeprom);

	UsartPuts(ptrInFlash);
	UsartPuts(ptrInEeprom);
	UsartPuts(strInRam);
}

