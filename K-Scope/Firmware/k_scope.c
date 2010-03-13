/* Name: main.c
 * Project: K-Scope, a USB oscilloscope
 * Author: Konstantin Chizhov
 * Creation Date: 2009-09-20
 * Copyright: (c) 2009 Konstantin Chizhov
 * License: GNU GPL v2 (see License.txt)
 */
 
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <avr/pgmspace.h>
#include "usbdrv.h"
#include "oddebug.h"
#include "requests.h"

enum
{ 
	adcBufferSize=500
};

#define MIN(v1, v2) ((v1)>(v2) ? (v2):(v1))

	unsigned char adc_buffer[adcBufferSize];
	unsigned char data_ready;
	unsigned char *ptr;


void ADCstop()
{
	ADMUX=0;
	ADCSRA=0;
}


void AdcGetPole(uint8_t adcsra, uint8_t admux, unsigned len)
{
		DDRC&=~(_BV(0) | _BV(1) | _BV(6) | _BV(7));
	
	admux |= _BV(ADLAR) | _BV(REFS0);
	adcsra &= _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2);
	ADMUX = admux;

	ADCSRA = _BV(ADFR) | _BV(ADSC) | _BV(ADEN) | adcsra;

	register unsigned char *p, *end;
	p=adc_buffer;
	end = adc_buffer + MIN(adcBufferSize, len);

	// discard first convertion
	while((ADCSRA&_BV(ADIF)) == 0)	wdt_reset();
	ADCSRA |= _BV(ADIF);
	*(p) = ADCH;

	do
	{
		while((ADCSRA&_BV(ADIF)) == 0)	wdt_reset();
		ADCSRA |= _BV(ADIF);
		*(p++) = ADCH;
	}while(p < end);

	ADCstop();
	data_ready=1;
}

void AdcGetPole2(uint8_t adcsra, uint8_t admux1, uint8_t admux2, unsigned len)
{
	DDRC&=~(_BV(0) | _BV(1) | _BV(6) | _BV(7));
	
	admux1 |= _BV(ADLAR) | _BV(REFS0);
	admux2 |= _BV(ADLAR) | _BV(REFS0);

	adcsra &= _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2);
	ADMUX = admux1;
	ADCSRA = /*_BV(ADATE) |*/ _BV(ADSC) | _BV(ADEN) | adcsra;

	register unsigned char *p, *end;
	p=adc_buffer;
	end = adc_buffer + MIN(adcBufferSize, len);

	// discard first convertion
	while((ADCSRA&_BV(ADIF)) == 0)	wdt_reset();
		ADCSRA |= _BV(ADIF);
		*(p) = ADCH;

	do
	{	
		ADMUX=admux1;
		ADCSRA |= _BV(ADSC);
		while((ADCSRA&_BV(ADIF)) == 0)	wdt_reset();
		*(p++) = ADCH;	

		ADMUX=admux2;
		ADCSRA |= _BV(ADSC);
		while((ADCSRA&_BV(ADIF)) == 0)	wdt_reset();
		*(p++) = ADCH;		

	}while(p < end);

	ADCstop();
	data_ready=1;
}

void AdcGetPole3(uint8_t adcsra, uint8_t admux1, uint8_t admux2, uint8_t admux3, unsigned len)
{
	DDRC&=~(_BV(0) | _BV(1) | _BV(6) | _BV(7));
	
	admux1 |= _BV(ADLAR) | _BV(REFS0);
	admux2 |= _BV(ADLAR) | _BV(REFS0);
	admux3 |= _BV(ADLAR) | _BV(REFS0);

	adcsra &= _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2);
	ADMUX = admux1;
	ADCSRA = /*_BV(ADATE) |*/ _BV(ADSC) | _BV(ADEN) | adcsra;

	register unsigned char *p, *end;
	p=adc_buffer;
	end = adc_buffer + MIN(adcBufferSize, len)-2;

	// discard first convertion
	while((ADCSRA&_BV(ADIF)) == 0)	wdt_reset();
		ADCSRA |= _BV(ADIF);
		*(p) = ADCH;

	do
	{	
		ADMUX=admux1;
		ADCSRA |= _BV(ADSC);
		while((ADCSRA&_BV(ADIF)) == 0)	wdt_reset();
		*(p++) = ADCH;	

		ADMUX=admux2;
		ADCSRA |= _BV(ADSC);
		while((ADCSRA&_BV(ADIF)) == 0)	wdt_reset();
		*(p++) = ADCH;	
		
		ADMUX=admux3;
		ADCSRA |= _BV(ADSC);
		while((ADCSRA&_BV(ADIF)) == 0)	wdt_reset();
		*(p++) = ADCH;

	}while(p < end);

	ADCstop();
	data_ready=1;
}


void AdcGetPole4(uint8_t adcsra, uint8_t admux1, uint8_t admux2, uint8_t admux3, uint8_t admux4, unsigned len)
{
	DDRC&=~(_BV(0) | _BV(1) | _BV(6) | _BV(7));
	
	admux1 |= _BV(ADLAR) | _BV(REFS0);
	admux2 |= _BV(ADLAR) | _BV(REFS0);
	admux3 |= _BV(ADLAR) | _BV(REFS0);
	admux4 |= _BV(ADLAR) | _BV(REFS0);

	adcsra &= _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2);
	ADMUX = admux1;
	ADCSRA = /*_BV(ADATE) |*/ _BV(ADSC) | _BV(ADEN) | adcsra;

	register unsigned char *p, *end;
	p=adc_buffer;
	end = adc_buffer + MIN(adcBufferSize, len)-3;

	// discard first convertion
	while((ADCSRA&_BV(ADIF)) == 0)	wdt_reset();
		ADCSRA |= _BV(ADIF);
		*(p) = ADCH;

	do
	{	
		ADMUX=admux1;
		ADCSRA |= _BV(ADSC);
		while((ADCSRA&_BV(ADIF)) == 0)	wdt_reset();
		*(p++) = ADCH;	

		ADMUX=admux2;
		ADCSRA |= _BV(ADSC);
		while((ADCSRA&_BV(ADIF)) == 0)	wdt_reset();
		*(p++) = ADCH;	
		
		ADMUX=admux3;
		ADCSRA |= _BV(ADSC);
		while((ADCSRA&_BV(ADIF)) == 0)	wdt_reset();
		*(p++) = ADCH;
		
		ADMUX=admux4;
		ADCSRA |= _BV(ADSC);
		while((ADCSRA&_BV(ADIF)) == 0)	wdt_reset();
		*(p++) = ADCH;	

	}while(p < end);

	ADCstop();
	data_ready=1;
}



uint16_t ADC_get(uint8_t adcsra, uint8_t admux)
{
	admux |= _BV(REFS0);
	adcsra &= _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2);
	ADMUX = admux;

	ADCSRA = _BV(ADSC) | _BV(ADEN) | adcsra;
	while (ADCSRA & _BV(ADSC));
	return ADC;
}

void WriteDacR2R(uint16_t value)
{
	PORTC = (uint8_t)(value >> 2);
	PORTD = (PORTD & (uint8_t)~3) | (uint8_t)(value & 3);
	DDRC = 0xff;
	DDRD |= 3;
}

usbMsgLen_t usbFunctionSetup(uchar data[8])
{

	usbRequest_t    *rq = (void *)data;
	uint8_t ch[]={1,2,6,7};

	switch(rq->bRequest)
	{

    case CUSTOM_RQ_GET_ADC_1_CHANEL:

		AdcGetPole(rq->wValue.bytes[0], 
					rq->wIndex.bytes[0], 
					MIN(adcBufferSize, rq->wLength.word));

		usbMsgPtr = adc_buffer;
        return MIN(adcBufferSize, rq->wLength.word);
	
	case CUSTOM_RQ_GET_ADC_2_CHANEL:

		AdcGetPole2(rq->wValue.bytes[0], 
					rq->wIndex.bytes[0], 
					rq->wIndex.bytes[1], 
					MIN(adcBufferSize, rq->wLength.word));

		usbMsgPtr = adc_buffer;
        return MIN(adcBufferSize, rq->wLength.word);

	case CUSTOM_RQ_GET_ADC_3_CHANEL:

		AdcGetPole3(rq->wValue.bytes[0], 
					rq->wIndex.bytes[0], 
					rq->wIndex.bytes[1],
					6,
					MIN(adcBufferSize, rq->wLength.word));

		usbMsgPtr = adc_buffer;
        return MIN(adcBufferSize, rq->wLength.word);

	case CUSTOM_RQ_GET_ADC_4_CHANEL:

		AdcGetPole4(rq->wValue.bytes[0], 
					rq->wIndex.bytes[0], 
					rq->wIndex.bytes[1],
					6, 7,
					MIN(adcBufferSize, rq->wLength.word));

		usbMsgPtr = adc_buffer;
        return MIN(adcBufferSize, rq->wLength.word);

	case CUSTOM_RQ_GET_BUFFER_SIZE:
		((uint16_t*)adc_buffer)[0] = adcBufferSize;
		usbMsgPtr = adc_buffer;
        return 2;

 	case CUSTOM_RQ_GET_CPU_FREQ:
		((uint32_t*)adc_buffer)[0] = F_CPU;
		usbMsgPtr = adc_buffer;
        return 4;

	case CUSTOM_RQ_GET_ADC_1_SAMPLE:
		
		for(uint8_t chanel=0; chanel<4; ++chanel)
			((uint16_t*)adc_buffer)[chanel] = ADC_get(rq->wValue.bytes[0], ch[chanel]);
		usbMsgPtr = adc_buffer;
        return 8;

	case CUSTOM_RQ_WRITE_DAC:
		WriteDacR2R(rq->wValue.word);
        return 0;

	case CUSTOM_RQ_PING:
		return 0;
	case CUSTOM_RQ_TRANSFER_TEST:
		usbMsgPtr = adc_buffer;
		return MIN(adcBufferSize, rq->wLength.word);
	}

    return 0;
}


void initTimer0()
{
	TCCR0 = _BV(CS01);
	TIMSK |= _BV(TOIE0);
}


ISR(TIMER0_COMP_vect)
{

}

ISR(TIMER0_OVF_vect, ISR_NOBLOCK)
{
	sei();
	static char value;
	PORTC=value+=2;
}


int main(void)
{
	uchar   i;
    wdt_enable(WDTO_1S);

	//odDebugInit();

	//initTimer0();
 	
	//DDRD |= 3; 

    usbInit();
    usbDeviceDisconnect();
    i = 0;
    while(--i){
        wdt_reset();
        _delay_ms(1);
    }
    usbDeviceConnect();
    
    sei();
    
    for(;;)
	{     
        wdt_reset();
        usbPoll();
	}
    return 0;
}

/* ------------------------------------------------------------------------- */
