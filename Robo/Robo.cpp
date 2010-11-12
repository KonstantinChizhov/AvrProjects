#include "main.h"

using namespace Timers;

template<unsigned IntValue>
struct IntToType
{
	enum{value = IntValue};
};


template<class Timer, unsigned long TickPerSecond, unsigned long Fcpu = F_CPU, unsigned DividerNum = 0>
struct DividerReloadValueIter
{
	typedef typename Timer::template Divider<DividerNum> DividerType;

	enum{Ticks = Fcpu/TickPerSecond/DividerType::value};

	typedef typename StaticIf<
		(Ticks >= Timer::MaxValue), 
		DividerReloadValueIter<Timer, TickPerSecond, Fcpu, DividerNum+1>,
		IntToType<Ticks> 
		>::Result ValueSelector;
	
	enum{value = Timer::MaxValue - ValueSelector::value};
};


template<class Timer, unsigned long TickPerSecond, unsigned long Fcpu = F_CPU, unsigned DividerNum = 0>
struct DividerIter
{
	typedef typename Timer::template Divider<DividerNum> DividerType;

	enum{Ticks = Fcpu/TickPerSecond/DividerType::value};
	
	typedef typename StaticIf<
		(Ticks >= Timer::MaxValue), 
		DividerIter<Timer, TickPerSecond, Fcpu, DividerNum+1>,
		DividerType
		>::Result DividerSelector;


	static const typename Timer::ClockDivider value = DividerSelector::value;
};


template<class Timer, unsigned long TickPerSecond, unsigned long Fcpu = F_CPU>
struct TimerFreqSetup
{
	enum{ReloadValue = DividerReloadValueIter<Timer, TickPerSecond, Fcpu, 0>::value};
	static const typename Timer::ClockDivider Divider = DividerIter<Timer, TickPerSecond, Fcpu, 0>::value;
};

ISR(SIG_INTERRUPT0)
{
	Ir::CaptureHandler();
}

ISR(SIG_OVERFLOW0)
{
	Ir::TimeoutHandler();
}

ISR(SIG_OUTPUT_COMPARE1A)
{
	Sheduller::TimerHandler();
	Encoder1::CaptureHandler();
}


int main()
{

	//Timer1::Start(Timer1::Div64);
	//Timer1::SetMode(Timer1::Normal);
	///Timer0::Start(Timer0::Div8);
//	Timer0::SetMode(Timer0::PwmPhaseCorrect);

//	Sheduller::Init();
	
typedef TimerFreqSetup<Timer0, 10000> T0Setup;
Timer0::Start(T0Setup::Divider);

Timer0::Set(T0Setup::ReloadValue);

	while(1)
	{
	//	Sheduller::Poll();
	}	
}
