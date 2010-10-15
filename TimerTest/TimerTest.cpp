#include "static_assert.h"
#include "static_switch.h"
#include <stdint.h>
#include <avr/io.h>

		enum class ClockSource
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

			void Stop()
			{
			
			}

			void Clear()
			{
			
			}

			template<ClockDivider divider>
			static void Start()
			{
				StaticSwitchValue
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
			}

			static void Start(NativeClockDivider divider)
			{
				TCCR0 = (divider&0x07 << CS00);
			}

			enum{CompareChannel = 2};
			template<int CompreUnit> class CcUnit
			{
				
			};
		};

int main()
{
	Timer0::Start<Cs1>();
return 0;
}
