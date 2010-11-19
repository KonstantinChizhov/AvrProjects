#include <ioavr.h>
#include <stdint.h>

static uint8_t EncState=0;
static volatile uint16_t EncValue=0;

#pragma inline=forced
inline static void EncoderScan(void)
{
	uint8_t newValue = PINC & 0x03;
	uint8_t fullState = newValue | EncState << 2;

	switch(fullState)
	{
		case 0x2: case 0x4: case 0xB: case 0xD:
			EncValue ++;
		break;
		case 0x1: case 0x7: case 0x8: case 0xE:
			EncValue --;
		break;
	}
	EncState = newValue;
}

#pragma vector=TIMER0_OVF_vect
__interrupt void TIMER0_OVF()
{
  EncoderScan();
}

int main()
{
  while(1)
  {
    PORTB = EncValue;
  }
  return 0;
}
