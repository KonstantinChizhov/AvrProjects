#pragma once

#ifdef TCNT0

class Timer0
{
public:
	typedef uint8_t DataT;
	enum ClockDiv{CkStop=0, Ck=1, Ck8=2, Ck64=3, Ck256=4, Ck1024=5, ExtFalling=6, ExRising=7};
	static void Start(ClockDiv div)
	{
		TCCR0 = (div << CS00);
	}

	static void Stop()
	{
		TCCR0 = CkStop;
	}

	static void Set(DataT val)
	{
		TCNT0 = val;
	}

	static DataT Get()
	{
		return TCNT0;
	}

	static void PrescalerReset()
	{
#ifdef PSR10
		SFIOR |= (1 << PSR10);
#endif
#ifdef PSR0
		TCCR0 |= (1 << PSR0);
#endif
	}

	static void Reset()
	{
#ifdef PSR0
		TCCR0 |= (1 << PSR0);
#endif
		TCNT0 = 0;
	}

	static void EnableOverflowInterrupt()
	{
		TIMSK |= (1 << TOIE0);
	}

	static bool Overflow()
	{
		return TIFR |= (1<<TOV0);
	}
#ifdef OCR0
	static void SetOutputCompare(DataT val)
	{
		OCR0 = val;
	}

	static void EnableOcrInterrupt()
	{
		TIMSK |= (1 << OCIE0);
	}

	static bool CompareMatch()
	{
		return TIFR & (1 << OCF0);
	}

	enum OcrMode{
			Normal = 0, 
			ToggleOC0 = (1 << COM00), 
			ClearOC0 = (1 << COM01), 
			SetOC0 = (1 << COM00) | (1 << COM01)
		};

	static void SetOcrMode(OcrMode mode)
	{
		TCCR0 = (TCCR0 & ~(1 << COM00) | (1 << COM01)) | mode;
	}

	enum WgmMode{
			WgmNormal = 0, 
			PWM = (1 << WGM00), 
			CTC = (1 << WGM01), 
			FastPWM = (1 << WGM00) | (1 << WGM01)
		};

	static void SetWGM(WgmMode mode)
	{
		TCCR0 = (TCCR0 & ~(1 << WGM00) | (1 << WGM01)) | mode;
	}
#endif
};

#endif //TCNT0

#ifdef TCNT1

class Timer1
{
public:
#ifdef TCNT1L //16 bit timer
	typedef uint16_t DataT; 
#else
	typedef uint8_t DataT;
#endif

	enum ClockDiv{
		Stop=0, 
		Ck=1, 
		Ck8=2, 
		Ck64=3, 
		Ck256=4, 
		Ck1024=5, 
		ExtFalling=6, 
		ExRising=7
	};

	static void Start(ClockDiv div)
	{
		TCCR1B =  (div << CS00);
	}

	static void Set(DataT val)
	{
		TCNT0 = val;
	}

	static DataT Get()
	{
		return TCNT0;
	}

	static void PrescalerReset()
	{
#ifdef PSR10
		SFIOR |= (1 << PSR10);
#endif
#ifdef PSR0
		TCCR0 |= (1 << PSR0);
#endif
	}

	static void Reset()
	{
#ifdef PSR0
		TCCR0 |= (1 << PSR0);
#endif
		TCNT0 = 0;
	}

	static void EnableOverflowInterrupt()
	{
		TIMSK |= (1 << TOIE0);
	}

	static bool Overflow()
	{
		return TIFR |= (1<<TOV0);
	}
#ifdef OCR0
	static void SetOutputCompare(DataT val)
	{
		OCR0 = val;
	}

	static void EnableOcrInterrupt()
	{
		TIMSK |= (1 << OCIE0);
	}

	static bool CompareMatch()
	{
		return TIFR & (1 << OCF0);
	}

	enum OcrMode{
			Normal = 0, 
			ToggleOC0 = (1 << COM00), 
			ClearOC0 = (1 << COM01), 
			SetOC0 = (1 << COM00) | (1 << COM01)
		};

	static void SetOcrMode(OcrMode mode)
	{
		TCCR0 = (TCCR0 & ~(1 << COM00) | (1 << COM01)) | mode;
	}

	enum WgmMode{
			WgmNormal = 0, 
			PWM = (1 << WGM00), 
			CTC = (1 << WGM01), 
			FastPWM = (1 << WGM00) | (1 << WGM01)
		};

	static void SetWGM(OcrMode mode)
	{
		TCCR0 = (TCCR0 & ~(1 << WGM00) | (1 << WGM01)) | mode;
	}
#endif
	protected:
};

#endif //TCNT1
