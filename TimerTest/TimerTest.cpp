#include "static_assert.h"
#include "static_switch.h"
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

		enum ClockSource
		{
			SystemClock,
			AuxiliaryClock,
			ExtFalling, 
			ExtRising, 
		};
		
		enum ClockDivider
		{
			Cs1, Cs2, Cs4, Cs8, 
			Cs16, Cs32, Cs64, Cs128, 
			Cs256, Cs512, Cs1024, Cs2048, 
			Cs4096, Cs8192, Cs16384
		};
						
		enum CountMode
		{	
			Stop,
			Normal,
			ClearOnCompareMatch0,
			UpAndDown
		};
		
		
		class Timer0
		{		
			public:
			typedef uint8_t DataT;
			enum NativeClockDivider
			{
				NativeCkStop=0, Native1=1, Native8=2, Native64=3, Native256=4, Native1024=5, NativeExtFalling=6, NativeExtRising=7
			};

			enum
			{
				ExternalClockPrescaled = 0,
				CompareChannel = 2,
				HasOverflowInterrupt = 1,
				HasCompareMatchInterrupt = 0
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

			template<ClockDivider divider>
			static void Start()
			{
				//Compile time convertion of global ClockDivider to NativeClockDivider
				const int nativeDivider = StaticSwitchValue
				<
					divider,
					Loki::TL::MakeTypelist
					<
						CaseValue<Cs1, Native1>,
						CaseValue<Cs8, Native8>,
						CaseValue<Cs64, Native1>,
						CaseValue<Cs256, Native1>,
						CaseValue<Cs1024, Native1>,
						CaseValue<NativeExtFalling, Native1>,
						CaseValue<NativeExtRising, Native1>
					>::Result,
					-1
				>::Value;
				// Compile time test if divider is applicable
				BOOST_STATIC_ASSERT(nativeDivider > 0);
				Start(NativeClockDivider(nativeDivider));
			}

			static void Start(NativeClockDivider divider)
			{
				TCCR0 |= (divider&0x07 << CS00);
			}

			static void EnableOverflowInterrupt()
			{
				TIMSK |= (1 << TOIE0);
			}

			static bool Overflow()
			{
				return TIFR |= (1<<TOV0);
			}
		};


ISR(TIMER0_OVF0_vect)
{

}

int main()
{
	Timer0::Start<Cs8>();
	Timer0::Stop();
	return 0;
}
