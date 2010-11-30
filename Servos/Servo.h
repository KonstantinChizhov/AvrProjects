#pragma once
#include <timer.h>

template<class Port, class Timer>
class Servo
{
	typedef Timers::TimerFreqSetup<Timer, 65> PauseSetup;
public:
	enum{Channels = Port::Length};

	static void Init()
	{
		Timer::Start(PauseSetup::Divider);
		Timer::Set(PauseSetup::ReloadValue);
		Timer::EnableInterrupt();
		for(uint8_t i=0; i< Channels; i++)
			values[i] = 127;
	}

	static void TimerHandler()
	{
	
	}

	static void Set(uint8_t channel, uint8_t value)
	{
	
	}

private:
	uint8_t values[Channels];
};
