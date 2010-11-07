
#pragma once	
	
namespace Timers
{
	class Timer1
	{
		public:
		typedef uint8_t DataT;
		enum ClockDivider
		{
			DivStop		= (0<<CS13) | (0<<CS12) | (0<<CS11) | (0<<CS10), 
			Div1 		= (0<<CS13) | (0<<CS12) | (0<<CS11) | (1<<CS10),
			Div2 		= (0<<CS13) | (0<<CS12) | (1<<CS11) | (0<<CS10),
			Div4 		= (0<<CS13) | (0<<CS12) | (1<<CS11) | (1<<CS10),
			Div8 		= (0<<CS13) | (1<<CS12) | (0<<CS11) | (0<<CS10),
			Div16 		= (0<<CS13) | (1<<CS12) | (0<<CS11) | (1<<CS10),
			Div32 		= (0<<CS13) | (1<<CS12) | (1<<CS11) | (0<<CS10),
			Div64	 	= (0<<CS13) | (1<<CS12) | (1<<CS11) | (1<<CS10),
			Div128	 	= (1<<CS13) | (0<<CS12) | (0<<CS11) | (0<<CS10),
			Div256	 	= (1<<CS13) | (0<<CS12) | (0<<CS11) | (1<<CS10),
			Div512	 	= (1<<CS13) | (0<<CS12) | (1<<CS11) | (0<<CS10),
			Div1024	 	= (1<<CS13) | (0<<CS12) | (1<<CS11) | (1<<CS10),
			Div2048	 	= (1<<CS13) | (1<<CS12) | (0<<CS11) | (0<<CS10),
			Div4096	 	= (1<<CS13) | (1<<CS12) | (0<<CS11) | (1<<CS10),
			Div8192	 	= (1<<CS13) | (1<<CS12) | (1<<CS11) | (0<<CS10),
			Div16384	= (1<<CS13) | (1<<CS12) | (1<<CS11) | (1<<CS10)
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
		
		static void ClearPrescaller()
		{
			TCCR1B |= (1 << PSR1);
		}
		
		enum{ClockDividerMask = ~((1<<CS13) | (1<<CS12) | (1<<CS11) | (1<<CS10))};
		
		static void Start(ClockDivider divider)
		{
			TCCR1B = (TCCR1B & ClockDividerMask) | divider;
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
		
		enum TimerMode
		{
			Normal			= (0 << PWM1A) | (0 << PWM1B) | (0 << CTC1),
			PwmA 			= (1 << PWM1A) | (0 << PWM1B) | (0 << CTC1),
			PwmB			= (0 << PWM1A) | (1 << PWM1B) | (0 << CTC1),
			PwmAPwmB 		= (1 << PWM1A) | (1 << PWM1B) | (0 << CTC1),
			ClearOnOcr1c 	= (0 << PWM1A) | (0 << PWM1B) | (1 << CTC1),
			PwmAToOcr1c 	= (1 << PWM1A) | (0 << PWM1B) | (1 << CTC1),
			PwmBToOcr1c 	= (0 << PWM1A) | (1 << PWM1B) | (1 << CTC1),
			PwmAPwmBToOcr1c = (1 << PWM1A) | (1 << PWM1B) | (1 << CTC1)
		};

		enum {TCCR1AClearMask 	= ~((1 << PWM1A) | (1 << PWM1B))};
		enum {TCCR1AMask 		= ((1 << PWM1A) | (1 << PWM1B))};
		enum {TCCR1BClearMask 	= ~(1 << CTC1)};
		enum {TCCR1BMask 		= (1 << CTC1)};


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
			OCR1C = val;
		}

		static DataT Get()
		{
			return OCR1C;
		}
	};

	template<> class Timer1::OutputCompare<1>
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
		
		enum OutputMode
		{
			Disconnected = (0 << COM1A1) | (0 << COM1A0),
			TogleOutput	 = (0 << COM1A1) | (1 << COM1A0),
			ClearOutput	 = (1 << COM1A1) | (0 << COM1A0),
			SetOutput	 = (1 << COM1A1) | (1 << COM1A0),
			NotInvertedPwm	= (0 << COM1A1) | (1 << COM1A0),
			NotInvertedPwmOneOutput	= (1 << COM1A1) | (0 << COM1A0),
			InvertedPwm		= (1 << COM1A1) | (1 << COM1A0)
		};
		
		enum {OutputModeMask = ~((1 << COM1A1) | (1 << COM1A0))};
		
		static void SetMode(OutputMode mode)
		{
			TCCR1A = (TCCR1A & OutputModeMask) | mode;
		}		
	};
	
	template<> class Timer1::OutputCompare<2>
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
		
		enum OutputMode
		{
			Disconnected = (0 << COM1B1) | (0 << COM1B0),
			TogleOutput	 = (0 << COM1B1) | (1 << COM1B0),
			ClearOutput	 = (1 << COM1B1) | (0 << COM1B0),
			SetOutput	 = (1 << COM1B1) | (1 << COM1B0),
			NotInvertedPwm	= (0 << COM1B1) | (1 << COM1B0),
			NotInvertedPwmOneOutput	= (1 << COM1B1) | (0 << COM1B0),
			InvertedPwm		= (1 << COM1B1) | (1 << COM1B0)
		};
		
		enum {OutputModeMask = ~((1 << COM1B1) | (1 << COM1B0))};
		
		static void SetMode(OutputMode mode)
		{
			TCCR1A = (TCCR1A & OutputModeMask) | mode;
		}		
	};

}
