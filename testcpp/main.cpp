//#include <avr/io.h>
#include "dispatcher.h"
//#include "usart.h"
//#include "adc.h"
//#include "HD44780.h"
#include "stepper.h"
#include "LedDisplay.h"
//#include "util.h"
#include "ports.h"

/*
Lcd
<
	TPin<Porta, 6>,
	TPin<Porta, 5>,
	TPin<Porta, 4>,
	PinsLcdDataBus
	<
		TPin<Porta, 3>,
		TPin<Porta, 2>,
		TPin<Porta, 1>,
		TPin<Porta, 0>
	>
> lcd;
*/
/*
Lcd
<
	TPin<Porta, 6>,
	TPin<Porta, 5>,
	TPin<Porta, 4>,
	PortLcdDataBus
	<
		Porta, 0
	>
> lcd;
*/
//TPin<Portb, 0> pin;
/*
LB1946
<
	TPin<Portc, 6>,//deta
	TPin<Portc, 7>,//clk
	TPin<Portc, 5>,//set
	TPin<Portc, 4>//enable
> mot;
*/

LedDisplay<PortSegmentsInv<Portb>, CommonsPortL<Portc, 4> > led;

int16_t count=0;
uint8_t dir=1;
const int maxCnt = 250;

SimpleStepper
<
	TPin<Portc, 4>,//in1
	TPin<Portc, 5>,//in2
	TPin<Portd, 7>,//e1
	TPin<Portc, 6>,//in3
	TPin<Portc, 7>,//in4
	TPin<Portd, 6>//e2
> mot;

void f()
{
	led.WriteDec(count);
	
	Dispatcher::SetTimer(f, 50);
}

void f2()
{
	led.Update();
	if(dir)
	{
		if(++count>=maxCnt)
		{
			dir=0;
		}
		mot.StepFwd();
	}
	else
	{
		if(--count<=0)
		{
			dir=1;
		}
		mot.StepBack();
	}
	Dispatcher::SetTimer(f2, 2);
}


int main()
{
 	Dispatcher::Init();
	sei();
	mot.Enable();

	f();
	f2();
	while(1)
	{	
		Dispatcher::Poll();
	}

}


