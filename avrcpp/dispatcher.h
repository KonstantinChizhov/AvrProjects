#include <avr/io.h>
#include "containers.h"
#include <avr/interrupt.h>
#include "util.h"


typedef void (*task_t)();

typedef struct
{
	task_t task;
	uint16_t period;
} timer_t;

template<uint8_t TasksLenght, uint8_t TimersLenght>
class Dispatcher
{
public:

	static void Init()
	{	
		_tasks.Clear();
		for(uint8_t i=0; i<_timers.Size(); i++)
		{
			_timers[i].task = 0;
			_timers[i].period = 0;
		}
	}

	static void SetTask(task_t task)
	{
		ATOMIC{	_tasks.Write(task);}
	}

	static void SetTimer(task_t task, uint16_t period) __attribute__ ((noinline))
	{
		uint8_t i_idle=0;
		ATOMIC
		{
			for(uint8_t i=0; i<_timers.Size(); i++)
			{
				if(_timers[i].task == 0)
				{
					i_idle = i;
				}
				if(_timers[i].task == task)
				{
					_timers[i].period = period;
					return;
				}
			}
			_timers[i_idle].task = task;
			_timers[i_idle].period = period;
		}
	}

	static void StopTimer(task_t task)
	{
		ATOMIC
		{
			for(uint8_t i=0; i<_timers.Size(); i++)
			{
				if(_timers[i].task == task)
				{
					_timers[i].task = 0;
					return;
				}
			}
		}
	}

	static void Poll()
	{
		task_t task;
		//NOTE: no beed to block task Queue here. This is the only place the Queue read.
		//cli();
		if(_tasks.Read(task))
		{
		//	sei();
			task();
		}
		//sei();
	}

	static void TimerHandler()
	{
		for(uint8_t i=0; i<_timers.Size(); i++)
		{
			if(_timers[i].task != 0 && --_timers[i].period == 0)
			{
				_tasks.Write(_timers[i].task); 
				_timers[i].task = 0;
			}
		}
	}

private:
	static Queue<TasksLenght, task_t> _tasks;
	static Array<TimersLenght, timer_t> _timers;
};

template<uint8_t TasksLenght, uint8_t TimersLenght>
Array<TimersLenght, timer_t> Dispatcher<TasksLenght, TimersLenght>::_timers;

template<uint8_t TasksLenght, uint8_t TimersLenght>
Queue<TasksLenght, task_t> Dispatcher<TasksLenght, TimersLenght>::_tasks;




