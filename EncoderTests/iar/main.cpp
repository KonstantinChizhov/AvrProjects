#include <ioavr.h>
#include <stdint.h>

static uint8_t EncState=0;
static volatile uint16_t EncValue=0;
#if 0
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
#else
	typedef unsigned EncValueType;
	enum{EncoderChannels = 8};
	static volatile EncValueType EncoderValues[EncoderChannels];
	static uint8_t _x1, _x2;

	static inline uint8_t Detect(uint8_t x1, uint8_t x2, uint8_t y1, uint8_t y2) 
	{
		//return ~(x1 | y2) & (x2 ^ y1) | x1 & y2 & ~(x2 & y1);
		return (x2 ^ y1) & ~(x1 ^ y2);
	}

	inline uint8_t EncRead1();
	inline uint8_t EncRead2();
        
#pragma inline=forced
	static inline void EncoderCapture()
	{
		uint8_t y1 = EncRead1();
		uint8_t y2 = EncRead2();

		uint8_t fwd  = Detect(_x1, _x2, y1, y2);
		uint8_t back = Detect(_x2, _x1, y2, y1);

		_x1 = y1;
		_x2 = y2;

		volatile EncValueType * ptr = EncoderValues;
		uint8_t i = EncoderChannels - 1;
		do
		{	
			if(fwd & 1)
				 (*ptr) ++;
			else 
			if(back & 1)
				(*ptr) --;
			ptr++;
			fwd >>= 1;
			back >>= 1;
		}while(i--);
	}
#pragma inline=forced
	inline uint8_t EncRead1()
	{
		return PINC;
	}
#pragma inline=forced
	inline uint8_t EncRead2()
	{
		return PIND;
	}
        
#pragma vector=TIMER1_COMPA_vect
__interrupt void TIMER1_COMPA()
{
  EncoderCapture();
}

#endif

int main()
{
  while(1)
  {
    //PORTB = EncValue;
    PORTA = EncoderValues[0];
  }
  return 0;
}
