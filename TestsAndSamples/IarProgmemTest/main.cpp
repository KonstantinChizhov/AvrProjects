#include <ioavr.h>
#include <pgmspace.h>
#include <stdint.h>
//__flash char str1[] = "Hello, world"; 

template<class T>
void Put2(const T begin, const T end)
{
  	for(T ptr = begin; ptr != end; ++ptr)
		PORTA = *ptr;
}

#define VIA_PTR 0

#if VIA_PTR
__attribute((noinline))
inline void Foo(uint64_t *v)
{
	(*v)++;
}
#endif

uint64_t value = 0;

int main()
{
#if VIA_PTR
	Foo(&value);
#else
	value++;
#endif

//  Put2(str1, str1 + 11);
  
  //PORTA = str[0];
  return 0;
}
