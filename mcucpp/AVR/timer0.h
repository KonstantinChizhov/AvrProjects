#pragma once	
	
namespace Timers
{
	class BaseTimer0
	{
		public:
		typedef uint8_t DataT;
		enum ClockDivider
		{
			DivStop=0, 
			Div1 		= (1<<CS00), 
			Div8 		= (1<<CS01), 
			Div64 		= (1<<CS00) | (1<<CS01), 
			Div256 		= (1<<CS02), 
			Div1024 	= (1<<CS02) | (1<<CS00), 
			ExtFalling	= (1<<CS02) | (1<<CS01), 
			ExtRising	= (1<<CS02) | (1<<CS01) | (1<<CS00)
		};
		static void Set(DataT val)
		{
			TCNT0 = val;
		}

		static DataT Get()
		{
			return TCNT0;
		}

		static void Stop()
		{
			TCCR0 = 0;
		}

		static void Clear()
		{
			TCNT0 = 0;
		}

		static void Start(ClockDivider divider)
		{
			TCCR0 |= (divider & 0x07 << CS00);
		}

		static void EnableInterrupt()
		{
			TIMSK |= (1 << TOIE0);
		}

		static bool IsInterrupt()
		{
			return TIFR & (1<<TOV0);
		}
		
		static void ClearInterruptFlag()
		{
			TIFR |= (1<<TOV0);
		}
	};

#if defined WGM00

	class Timer0 : public BaseTimer0
	{		
		public:
	
		enum TimerMode
		{
			Normal = 0,
			Pwm = (1 << WGM00),
			Ctc = (1  << WGM01),
			FastPwm = (1 << WGM01) | (1 << WGM00)
		};

		enum {TimerModeClearMask = ~(1 << WGM01) | (1 << WGM00)};

		static void SetMode(TimerMode mode)
		{
			TCCR0 = (TCCR0 & TimerModeClearMask) | mode;
		}

		template<int number> class OutputCompare;
	};

	template<> class Timer0::OutputCompare<0>
	{
	public:
		static void Set(DataT val)
		{
			OCR0 = val;
		}

		static DataT Get()
		{
			return OCR0;
		}

		static void EnableInterrupt()
		{
			TIMSK |= (1 << OCIE0);
		}

		static bool IsInterrupt()
		{
			return TIFR & (1<<OCF0);
		}
	
		static void ClearInterruptFlag()
		{
			TIFR |= (1<<OCF0);
		}
	};
#else
	class Timer0 : public BaseTimer0
	{
	};
#endif

}
