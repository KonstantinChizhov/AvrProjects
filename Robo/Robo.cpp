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
//	Sheduller::TimerHandler();
	Encoder1::CaptureHandler();
}
*/

static volatile uint8_t EncState=0;
static volatile uint16_t EncData=0;
static volatile uint16_t EncData2=0;
uint8_t x1, x2;

void EncoderScan(void)
{

uint8_t New = PINB & 0x03;	
 
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



void Enc2()
{
	uint8_t y1 , y2;
	y1 = PORTB;
	y2 = PORTA;

	uint8_t fwd  = ~(x1 | y2) & (x2 ^ y1) | x1 & y2 & ~(x2 & y1);
	uint8_t back = ~(x2 | y1) & (x1 ^ y2) | x2 & y1 & ~(x1 & y2);
	if(fwd&1)EncData++;
	if(back&1)EncData--;
	if(fwd&2)EncData2++;
	if(back&2)EncData2--;
	x1 = y1;
	x2 = y2;
}

int main()
{

//	Timer1::Start(Timer1::Div64);
//	Timer1::SetMode(Timer1::Normal);

//	Sheduller::Init();
	
//	Timer0::Start(T0Setup::Divider);
//	Timer0::Set(T0Setup::ReloadValue);

//	EncoderScan();

	while(1)
	{
		Enc2();
		PORTA = EncData;
//		Sheduller::Poll();
	}	
}
