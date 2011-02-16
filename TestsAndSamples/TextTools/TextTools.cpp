#include <avr/io.h>
#include <flashptr.h>

template<class T>
void UsartPuts(T str)
{
  	while(char tmp = *str)
	{
		while(UCSRA & (1<<UDRE));
		UDR = tmp;
		str++;
	}
}


PROGMEM char Str[] = "Hello world!\n";

int main()
{

	ProgmemPtr<char> pStr(Str);
	// put string for flash
	UsartPuts(ProgmemPtr<char>(Str));
	// from flash again
	UsartPuts(ProgmemPtr<char>(PSTR("From flash\n")));
	// and from ram
	UsartPuts("From ram");

	while(1);
}
