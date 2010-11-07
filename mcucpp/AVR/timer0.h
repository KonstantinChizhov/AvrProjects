#pragma once	
	
namespace Timers
{

#ifdef TIMSK0
#define InterruptMaskReg TIMSK0
#else
#define InterruptMaskReg TIMSK
#endif

#ifdef TIFR0
#define InterruptFlagsReg TIFR0
#else
#define InterruptFlagsReg TIFR
#endif

#ifdef TCCR0A
#define ControlRegA TCCR0A
#else
#define ControlRegA TCCR0
#endif

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

		enum {ClockDividerMask = ~((1<<CS02) | (1<<CS01) | (1<<CS00))};

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
			ControlRegA = 0;
		}

		static void Clear()
		{
			TCNT0 = 0;
		}
		/*
		static void ClearPrescaller()
		{
			SFIOR |= (1 << PSR10);
		}*/

		static void Start(ClockDivider divider)
		{
			ControlRegA = (ControlRegA & ClockDividerMask) | divider;
		}

		static void EnableInterrupt()
		{
			InterruptMaskReg |= (1 << TOIE0);
		}

		static bool IsInterrupt()
		{
			return InterruptFlagsReg & (1<<TOV0);
		}
		
		static void ClearInterruptFlag()
		{
			InterruptFlagsReg |= (1<<TOV0);
		}
	};

#if defined WGM00	//Has waveform generation modes

	class Timer0 : public BaseTimer0
	{		
		public:
	
		enum TimerMode
		{
			Normal 			= (0 << WGM01) | (0 << WGM00),
			PwmPhaseCorrect = (0 << WGM01) | (1 << WGM00),
			ClearOnMatch 	= (1 << WGM01) | (0 << WGM00),
			PwmFast 		= (1 << WGM01) | (1 << WGM00)
		};

		enum {TimerModeClearMask = ~(1 << WGM01) | (1 << WGM00)};

		static void SetMode(TimerMode mode)
		{
			ControlRegA = (ControlRegA & TimerModeClearMask) | mode;
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
			InterruptMaskReg |= (1 << OCIE0);
		}

		static bool IsInterrupt()
		{
			return InterruptFlagsReg & (1<<OCF0);
		}
	
		static void ClearInterruptFlag()
		{
			InterruptFlagsReg |= (1<<OCF0);
		}
	};
#else
	class Timer0 : public BaseTimer0
	{
	};
#endif


#undef InterruptMaskReg 
#undef InterruptFlagsReg 
#undef ControlRegA


}
