#include <avr/io.h>
#include "containers.h"
#include <avr/interrupt.h>

#define TASKS_LENGTH 8
#define TIMERS_LENGTH 5

#define TIMER_INTERVAL 187
#define TIMER_START (256-TIMER_INTERVAL)

typedef void (*task_t)();

typedef struct
{
	task_t task;
	uint16_t period;
} timer_t;


class Dispatcher
{
public:
	static void Init()
	{
		for(uint8_t i=0; i<_timers.Size(); i++)
		{
			_timers[i].task = 0;
			_timers[i].period = 0;
		}
		TCNT0 = TIMER_START;
		TCCR0 = (0<<CS02) | (1<<CS01) | (1<<CS00);
		TIMSK |= (1<<TOIE0);
		sei();
	}

	static uint16_t Ticks()
	{
		return TCNT0-TIMER_START + _ticks;
	}

	static void SetTask(task_t task) //__attribute__ ((noinline))
	{
		//uint8_t statusCopy = SREG;
		//cli();
		_tasks.Write(task);
		//SREG = statusCopy;
	}

	static void SetTimer(task_t task, uint16_t period) __attribute__ ((noinline))
	{
		uint8_t i_idle=0;
		cli();
		uint8_t statusCopy = SREG;
		for(uint8_t i=0; i<_timers.Size(); i++)
		{
			if(_timers[i].task == 0)
			{
				i_idle = i;
			}
			if(_timers[i].task == task)
			{
				_timers[i].period = period;
				SREG = statusCopy;
				return;
			}
		}
		_timers[i_idle].task = task;
		_timers[i_idle].period = period;
		SREG = statusCopy;
	}

	static void StopTimer(task_t task)
	{
		cli();
		uint8_t statusCopy = SREG;
		for(uint8_t i=0; i<_timers.Size(); i++)
		{
			if(_timers[i].task == task)
			{
				_timers[i].task = 0;
				SREG = statusCopy;
				return;
			}
		}
		SREG = statusCopy;
	}

	static void Poll()
	{
		task_t task;
		cli();
		if(_tasks.Read(task))
		{
			sei();
			task();
		}
		sei();
	}

	static void TimerHandler()
	{
		_ticks+=TIMER_INTERVAL;
		for(uint8_t i=0; i<_timers.Size(); i++)
		{
			if(_timers[i].task != 0 && --_timers[i].period == 0)
			{
				_tasks.Write(_timers[i].task); 
				_timers[i].task = 0;
			}
		}
	}

protected:

	static Queue<TASKS_LENGTH, task_t> _tasks;
	static Array<TIMERS_LENGTH, timer_t> _timers;
	static uint16_t _ticks;

};

	Queue<TASKS_LENGTH, task_t> Dispatcher::_tasks;
	Array<TIMERS_LENGTH, timer_t> Dispatcher::_timers;
	uint16_t Dispatcher::_ticks;

ISR(TIMER0_OVF_vect)
{
	TCNT0 = TIMER_START;
	Dispatcher::TimerHandler();
}


