#include "static_assert.h"
#include <stdint.h>

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
			typedef uint16_t DataT;
			void Stop()
			{}
			void Clear()
			{
			
			}
			template<ClockSource clockSource, ClockDivider divider>
			static void Start()
			{
				BOOST_STATIC_ASSERT(divider != Cs2);
				BOOST_STATIC_ASSERT(divider != Cs512);
			}

			enum{CompareChannel = 2};
			template<int CompreUnit> class CcUnit
			{
				
			};
		};

int main()
{
	Timer0::Start<SystemClock, Cs4>();
return 0;
}
