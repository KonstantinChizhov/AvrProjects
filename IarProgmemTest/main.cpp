#include <ioavr.h>
#include <pgmspace.h>

__flash char str1[] = "Hello, world"; 

template<class T>
void Put2(const T begin, const T end)
{
  	for(T ptr = begin; ptr != end; ++ptr)
		PORTA = *ptr;
}

int main()
{

  Put2(str1, str1 + 11);
  
  //PORTA = str[0];
  return 0;
}
