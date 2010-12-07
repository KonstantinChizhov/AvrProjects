#include <ST/iostm32f10xxB.h>

 #pragma inline=forced
inline unsigned Unpack(unsigned value)
{
	value = (value & 0xfffffffe) << 3 | value & 0x1;
	value = (value & 0xffffffe0) << 3 | value & 0x11;
	value = (value & 0xfffffe00) << 3 | value & 0x111;
	value = (value & 0xffffe000) << 3 | value & 0x1111;
	value = (value & 0xfffe0000) << 3 | value & 0x11111;
	value = (value & 0xffe00000) << 3 | value & 0x111111;
	value = (value & 0xfe000000) << 3 | value & 0x1111111;
	
	return value;
}

int main()
{
  GPIOA_CRL  = Unpack(0xf0);
  GPIOA_CRH  = Unpack(0xff);
  return 0;
}
