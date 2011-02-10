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


	PROGMEM char Str[] = "Hello world";

int main()
{

/*	ProgmemPtr<char> pStr(Str), tmp;
	tmp = pStr + 11;
	while(pStr  != tmp)
	{
		while(UCSRA & (1<<UDRE));
		UDR = *pStr++;
	}*/

	ProgmemPtr<char> pStr(Str);
	
	UsartPuts(ProgmemPtr<char>(Str));
	UsartPuts("12345");

	while(1);
}
