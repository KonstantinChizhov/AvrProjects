#pragma once	
	
namespace Timers
{
	class BaseTimer1
	{
		public:
		typedef uint16_t DataT;
		enum ClockDivider
		{
			DivStop=0, 
			Div1 		= (1<<CS10), 
			Div8 		= (1<<CS11), 
			Div64 		= (1<<CS10) | (1<<CS11), 
			Div256 		= (1<<CS12), 
			Div1024 	= (1<<CS12) | (1<<CS00), 
			ExtFalling	= (1<<CS12) | (1<<CS11), 
			ExtRising	= (1<<CS12) | (1<<CS11) | (1<<CS10)
		};
		static void Set(DataT val)
		{
			TCNT1 = val;
		}

		static DataT Get()
		{
			return TCNT1;
		}

		static void Stop()
		{
			TCCR1B = 0;
		}

		static void Clear()
		{
			TCNT1 = 0;
		}

		static void Start(ClockDivider divider)
		{
			TCCR1B |= (divider & 0x07 << CS10);
		}

		static void EnableInterrupt()
		{
			TIMSK |= (1 << TOIE1);
		}

		static bool IsInterrupt()
		{
			return TIFR & (1<<TOV1);
		}
		
		static void ClearInterruptFlag()
		{
			TIFR |= (1<<TOV1);
		}
	};

#if defined WGM10

	class Timer1 : public BaseTimer1
	{		
		public:
	
		enum TimerMode
		{
			Normal = 0,
			PwmPhaseCorrect8Bit = (1 << WGM10),
			PwmPhaseCorrect9Bit = (1 << WGM11),
			PwmPhaseCorrect10Bit = (1 << WGM10) | (1 << WGM11),
			CtcToOcr1a = (1 << WGM12),
			PwmFast8Bit = (1 << WGM12) | (1 << WGM10),
			PwmFast9Bit = (1 << WGM12) | (1 << WGM11),
			PwmFast10Bit = (1 << WGM12) | (1 << WGM11) | (1 << WGM10),
			PwmPhaseAndFreqCorrectToIcr1 = (1 << WGM13),
			PwmPhaseAndFreqCorrectToOcr1a = (1 << WGM13) | (1 << WGM10),
			PwmPhaseCorrectToIcr1 = (1 << WGM13) | (1 << WGM11),
			PwmPhaseCorrectToOcr1a = (1 << WGM13) | (1 << WGM11) | (1 << WGM10),
			CtcToIcr1 = (1 << WGM13) | (1 << WGM12),
			// Reserved = (1 << WGM13) | (1 << WGM12) | (1 << WGM10),
			PwmFastToIcr1 = (1 << WGM13) | (1 << WGM12) | (1 << WGM11),
			PwmFastToOcr1a = (1 << WGM13) | (1 << WGM12) | (1 << WGM11) | (1 << WGM10)
		};

		enum {TCCR1AClearMask = ~((1 << WGM11) | (1 << WGM10))};
		enum {TCCR1AMask = ((1 << WGM11) | (1 << WGM10))};
		enum {TCCR1BClearMask = ~((1 << WGM12) | (1 << WGM13))};
		enum {TCCR1BMask = ((1 << WGM12) | (1 << WGM13))};

		static void SetMode(TimerMode mode)
		{
			TCCR1A = (TCCR0 & TCCR1AClearMask) | (mode & TCCR1AMask);
			TCCR1B = (TCCR0 & TCCR1BClearMask) | (mode & TCCR1BMask);
		}

		template<int number> class OutputCompare;
	};

	template<> class Timer1::OutputCompare<0>
	{
	public:
		static void Set(DataT val)
		{
			OCR1A = val;
		}

		static DataT Get()
		{
			return OCR1A;
		}

		static void EnableInterrupt()
		{
			TIMSK |= (1 << OCIE1A);
		}

		static bool IsInterrupt()
		{
			return TIFR & (1<<OCF1A);
		}
	
		static void ClearInterruptFlag()
		{
			TIFR |= (1<<OCF1A);
		}
	};
	
	template<> class Timer1::OutputCompare<1>
	{
	public:
		static void Set(DataT val)
		{
			OCR1B = val;
		}

		static DataT Get()
		{
			return OCR1B;
		}

		static void EnableInterrupt()
		{
			TIMSK |= (1 << OCIE1B);
		}

		static bool IsInterrupt()
		{
			return TIFR & (1<<OCF1B);
		}
	
		static void ClearInterruptFlag()
		{
			TIFR |= (1<<OCF1B);
		}
	};
	
#else
	class Timer1 : public BaseTimer1
	{
	};
#endif

}
