#pragma once	
	
namespace Timers
{
	class BaseTimer2
	{
		public:
		typedef uint8_t DataT;
		enum ClockDivider
		{
			DivStop		= (0<<CS22) | (0<<CS21) | (0<<CS20), 
			Div1 		= (0<<CS22) | (0<<CS21) | (1<<CS20),
			Div8 		= (0<<CS22) | (1<<CS21) | (0<<CS20),
			Div32 		= (0<<CS22) | (1<<CS21) | (1<<CS20),
			Div64 		= (1<<CS22) | (0<<CS21) | (0<<CS20),
			Div128 		= (1<<CS22) | (0<<CS21) | (1<<CS20),
			Div256 		= (1<<CS22) | (1<<CS21) | (0<<CS20),
			Div1024 	= (1<<CS22) | (1<<CS21) | (1<<CS20)
		};

		enum {ClockDividerMask = ~((1<<CS22) | (1<<CS21) | (1<<CS20))};

		static void Set(DataT val)
		{
			TCNT2 = val;
		}

		static DataT Get()
		{
			return TCNT2;
		}

		static void Stop()
		{
			TCCR2 = 0;
		}

		static void Clear()
		{
			TCNT2 = 0;
		}
		
		static void ClearPrescaller()
		{
			SFIOR |= (1 << PSR2);
		}

		static void Start(ClockDivider divider)
		{
			TCCR2 = (TCCR2 & ClockDividerMask) | divider;
		}

		static void EnableInterrupt()
		{
			TIMSK |= (1 << TOIE2);
		}

		static bool IsInterrupt()
		{
			return TIFR & (1<<TOV2);
		}
		
		static void ClearInterruptFlag()
		{
			TIFR |= (1<<TOV2);
		}
	};

#if defined WGM20

	class Timer2 : public BaseTimer2
	{		
		public:
	
		enum TimerMode
		{
			Normal 			= (0 << WGM21) | (0 << WGM20),
			PwmPhaseCorrect = (0 << WGM21) | (1 << WGM20),
			ClearOnMatch	= (1 << WGM21) | (0 << WGM20),
			PwmFast 		= (1 << WGM21) | (1 << WGM20)
		};

		enum {TimerModeClearMask = ~(1 << WGM21) | (1 << WGM20)};

		static void SetMode(TimerMode mode)
		{
			TCCR2 = (TCCR2 & TimerModeClearMask) | mode;
		}

		template<int number> class OutputCompare;
	};

	template<> class Timer2::OutputCompare<0>
	{
	public:
		static void Set(DataT val)
		{
			OCR2 = val;
		}

		static DataT Get()
		{
			return OCR2;
		}

		static void EnableInterrupt()
		{
			TIMSK |= (1 << OCIE2);
		}

		static bool IsInterrupt()
		{
			return TIFR & (1<<OCF2);
		}
	
		static void ClearInterruptFlag()
		{
			TIFR |= (1<<OCF2);
		}
	};
#else
	class Timer2 : public BaseTimer2
	{
	};
#endif

}
