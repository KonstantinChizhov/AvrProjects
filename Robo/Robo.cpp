#include "main.h"

using namespace Timers;
/*
ISR(SIG_INTERRUPT0)
{
	Ir::CaptureHandler();
}

ISR(SIG_OVERFLOW0)
{
	Ir::TimeoutHandler();
}


ISR(SIG_OUTPUT_COMPARE1A)
{
	Sheduller::TimerHandler();
	Encoder1::CaptureHandler();
}
*/

/*
static volatile uint8_t EncState=0;
static volatile uint16_t EncData=0;

uint8_t _x1, _x2;

void EncoderScan(void)
{

uint8_t New = PINC & 0x03;	
 
switch(EncState)
	{
	case 2:
		{
		if(New == 3) EncData++;
		if(New == 0) EncData--; 
		break;
		}
 
	case 0:
		{
		if(New == 2) EncData++;
		if(New == 1) EncData--; 
		break;
		}
	case 1:
		{
		if(New == 0) EncData++;
		if(New == 3) EncData--; 
		break;
		}
	case 3:
		{
		if(New == 1) EncData++;
		if(New == 2) EncData--; 
		break;
		}
	}
 
	EncState = New;

}
/*
/*
const unsigned EncCount = 8;
static volatile uint16_t Value[EncCount];

inline uint8_t Detect(uint8_t x1, uint8_t x2, uint8_t y1, uint8_t y2) 
{
	return ~(x1 | y2) & (x2 ^ y1) | x1 & y2 & ~(x2 & y1);
}

void Enc2()
{
	uint8_t y1 , y2;
	y1 = PINC;
	y2 = PIND;

	uint8_t fwd  = Detect(_x1, _x2, y1, y2);
	uint8_t back = Detect(_x2, _x1, y2, y1);
	
	for(unsigned i = 0; i<EncCount; ++i)
	{
		if(fwd&1)
			Value[i]++;
		else if(back&1)
			Value[i]--;
		fwd >>= 1;
		back >>= 1;
	}

	_x1 = y1;
	_x2 = y2;
}
*/

int main()
{
	DDRB = 0xff;
	//Timer1::Start(Timer1::Div64);
	//Timer1::SetMode(Timer1::Normal);

	//Sheduller::Init();
	
	//Timer0::Start(T0Setup::Divider);
	//Timer0::Set(T0Setup::ReloadValue);

//	

	while(1)
	{
		//EncoderScan();
//		Enc2();
		Encoder1::CaptureHandler();

		PORTB = Encoder1::Value(0)>>8;
		//RTB = EncData;
		//Sheduller::Poll();
	}	
}
