#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>
#include "lcd.h"

static const IrPulseThershold = 9;
static const uint8_t TimerReloadValue = 150;
static const uint8_t TimerClock = (1 << CS02) | (1 << CS00);// 8 MHz / 1024

volatile struct ir_t
{
	uint8_t rx_started;
	uint32_t code, rx_buffer;
} ir;

static void ir_start_timer()
{
	TIMSK = (1 << TOIE0);
	TCNT0 = 0;
	TCCR0 = TimerClock;
}

ISR(TIMER0_OVF0_vect)
{
	ir.code = ir.rx_buffer;
	ir.rx_buffer = 0;
	ir.rx_started = 0;
	if(ir.code == 0)
		TCCR0 = 0;
	TCNT0 = TimerReloadValue;
}

ISR(INT0_vect)
{
	uint8_t delta;
	if(ir.rx_started)
	{
		delta = TCNT0 - TimerReloadValue;
		ir.rx_buffer <<= 1;
		if(delta > IrPulseThershold) ir.rx_buffer |= 1;
	}
	else{
		ir.rx_started = 1;
	ir_start_timer();
	}
	TCNT0 = TimerReloadValue;
}

static inline void ir_init()
{
	GIMSK |= _BV(INT0);
	MCUCR |= (1 << ISC00) | (0 <<ISC01);
	ir_start_timer();
}

char buf[10];

int main()
{
	PORTA|=_BV(0) | _BV(1);
	DDRA=0; 
	DDRB|=_BV(PB3) | _BV(PB5);
	PORTB|=_BV(PB5);

	ir_init();
	lcd_init();
	sei();
	lcd_puts("IR Lcd");

	for(;;)
	{
		if(ir.code)
		{
			ultoa(ir.code, buf, 16);
			lcd_clear();
			lcd_puts((buf));
		}
		
		PORTB^=_BV(PB3);
	}
}
