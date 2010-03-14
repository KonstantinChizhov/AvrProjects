#ifndef ADC_HPP
#define ADC_HPP
#include <avr/io.h>
#include <avr/interrupt.h>


enum Vref{External=0, VCC=1, Int2_56=3};
enum AdcDivider{Dev2=1, Dev4=2, Dev8=3, Dev16=4, Dev32=5, Dev64=6, Dev128=7};


class Adc
{
public:
	enum{ADC_FREE_RUN=5};
	static void SetClockDivider(AdcDivider devider)
	{
		ADCSRA |= devider & 0x07;
	}

	static void SetVref(Vref ref)
	{
		ADMUX |= (ref << REFS0);
	}

	static void SetChannel(uint8_t channel)
	{
		ADMUX |= (channel & 0x1f) << MUX0;
	}
	 
	static void Init(uint8_t channel, Vref ref)
	{
		ADMUX |= ((channel & 0x1f) << MUX0) | (ref << REFS0);
	}
	
	static void StartFreeRun()
	{
		ADCSRA |= _BV(ADC_FREE_RUN) | _BV(ADSC) | _BV(ADEN);
	}

	static void StartFreeRunInterrupt()
	{
		ADCSRA |= _BV(ADC_FREE_RUN) | _BV(ADIE) | _BV(ADSC) | _BV(ADEN);
	}

	static uint16_t ReadSingle()
	{
		ADCSRA |= _BV(ADSC) | _BV(ADEN);
		while (ADCSRA & _BV(ADSC));
		return ADC;
	}

	static void Stop()
	{
		ADMUX=0;
		ADCSRA=0;
	}

protected:

};


#endif
