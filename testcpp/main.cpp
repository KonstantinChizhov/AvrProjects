//#include <avr/io.h>
#include "dispatcher.h"
#include "usart.h"
#include "adc.h"
#include "HD44780.h"
#include "stepper.h"
#include "LedDisplay.h"
#include "util.h"
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

//TPin<Portb, 0> pin;

LB1946
<
	TPin<Portc, 6>,//deta
	TPin<Portc, 7>,//clk
	TPin<Portc, 5>,//set
	TPin<Portc, 4>//enable
> mot;


LedDisplay<PortSegmentsInv<Portb>, CommonsPortL<Portc, 4> > led;

void f()
{
	led.WriteDec(Adc::ReadSingle()-512);
	//Portb::data() = 0xff;
	//pin.Togle();
	Dispatcher::SetTimer(f, 50);
}

void f2()
{
	led.Update();
	if(Adc::ReadSingle() > 512)
		mot.HalfStepFwd();
	else
		mot.StepFwd();
	Dispatcher::SetTimer(f2, 1);
}

int main()
{
 	lcd.Puts("1234567890abcdef", 16);
 	//Usart::Init(115200);
	//TextFormater<WaitAdapter<Usart> > usart;
	//Usart usart;
	Adc::SetClockDivider(Dev32);
	Adc::SetChannel(0);
	Adc::SetVref(VCC);


	Dispatcher::Init();
	//pin.SetDirWrite();
	Portb::dir() = 0xff;

	sei();
	uint32_t i;
	uint32_t r;
	
	f();
f2();
	while(1)
	{	
		//usart.Getch(c);
		//usart.Putch(c);
		//usart >> i;
		Dispatcher::Poll();
			//usart << div10(i, r) << "\t" << r << "\r\n";

	}

}
