#include <ioavr.h>
#include <pgmspace.h>
#include <stdint.h>

__flash char strInFlash[] = "Hello from flash"; 
char strInRam[] = "Hello from RAM";
__eeprom char strInEeprom[] = "Hello from EEPROM";

static inline void UsartWriteByte(char value)
{
	while(!UCSRA_UDRE);
	UDR = value;
}

template<class T>
void UsartPuts(const T * str)
{
  	char c;
 	while(c = *str++)
	{
  		UsartWriteByte(c);
	}
}

int main()
{
	UsartPuts(strInFlash);
	UsartPuts(strInRam);
	UsartPuts(strInEeprom);
	
	return 0;
}
