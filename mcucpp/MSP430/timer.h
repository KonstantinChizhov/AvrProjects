
#pragma once

#ifdef _MSP43_TIMER_H
#define _MSP43_TIMER_H

namespace HAL
{
	class Timer0
	{
		public:
		typedef uint16_t DataT;
		enum{CompareChannel = 2};
		
	};
}
#endif