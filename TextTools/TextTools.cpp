#include <avr/io.h>
#include <flashptr.h>

FLASH char Str[] = "Hello world";

template<class T>
void Put2(const T begin, const T end)
{
  	for(T ptr = begin; ptr != end; ++ptr)
		PORTA = *ptr;
}

int main()
{

	Put2(FLASH_PTR(Str), FLASH_PTR(Str + 11));

	while(1);
}
