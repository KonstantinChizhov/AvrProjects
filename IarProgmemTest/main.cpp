#include <ioavr.h>
#include <pgmspace.h>

__flash char str1[] = "Message 1"; 


template<class T>
void Put(T s)
{
  	while(*s)
		PORTA = *s++; 
}

int main()
{
  static __flash char str2[] = "Message 2"; 
  //msg=str1; 
  Put(str1);
  Put(str2);
  //PORTA = str[0];
  return 0;
}
