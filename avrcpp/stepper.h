#pragma once
#ifndef STEP_MOTOR_HPP
#define STEP_MOTOR_HPP
#include <avr/pgmspace.h>


template <class DATA_PIN, class CLK_PIN, class SET_PIN, class ENABLE_PIN>
class LB1946Base
{
public:
	LB1946Base()
	{
		Init();
	}

	// writes data to LB1946 motor driver
	// a and b in format of:
	// [x][x][i][i][i][i][d][p]
	// 		i - chanel a current 
	//		d - decay mode 1/0 - fast/slow
	// 		p - polarity 1/0 - forward/reverce
	// 		x - not used
	
	static void Write(uint8_t a, uint8_t b)
	{
		_clk.Clear();
		_set.Clear();
		_delay_us(1);

		Write6(a);
		Write6(b);
		
		_set.Set();
		_delay_us(5);
		_set.Clear();
		_delay_us(_tD);
	}

	static void Enable()
	{
		_enable.Set();
	}

	static void Disable()
	{
		_enable.Clear();
	}

protected:

	static const int _tD = 2;

	static void Init()
	{
		_enable.Clear();
	
		_data.SetDirWrite();
		_set.SetDirWrite();
		_enable.SetDirWrite();
		_clk.SetDirWrite();
	}

	static void Write6(uint8_t v)
	{
		for(uint8_t i=0; i < 6; ++i, v <<= 1)
		{
			_data.Set((v & 0x20) != 0);
			_clk.Set();
			_delay_us(_tD);
			_clk.Clear();
			_delay_us(_tD);
		}
	}

	static DATA_PIN _data;
	static CLK_PIN _clk;
	static SET_PIN _set;
	static ENABLE_PIN _enable;
};

	template <class DATA_PIN, class CLK_PIN, class SET_PIN, class ENABLE_PIN>
	DATA_PIN LB1946Base<DATA_PIN, CLK_PIN, SET_PIN, ENABLE_PIN>::_data;

	template <class DATA_PIN, class CLK_PIN, class SET_PIN, class ENABLE_PIN>
	CLK_PIN LB1946Base<DATA_PIN, CLK_PIN, SET_PIN, ENABLE_PIN>::_clk;

	template <class DATA_PIN, class CLK_PIN, class SET_PIN, class ENABLE_PIN>
	SET_PIN LB1946Base<DATA_PIN, CLK_PIN, SET_PIN, ENABLE_PIN>::_set;

	template <class DATA_PIN, class CLK_PIN, class SET_PIN, class ENABLE_PIN>
	ENABLE_PIN LB1946Base<DATA_PIN, CLK_PIN, SET_PIN, ENABLE_PIN>::_enable;


template <class DATA_PIN, class CLK_PIN, class SET_PIN, class ENABLE_PIN>
class LB1946 :public LB1946Base<DATA_PIN, CLK_PIN, SET_PIN, ENABLE_PIN>
{
public:
	typedef LB1946Base<DATA_PIN, CLK_PIN, SET_PIN, ENABLE_PIN> Base;

	static void HalfStepFwd()
	{
		_phase = (_phase + 1) & 0x7;
		SetOutput();
	}

	static void HalfStepBack()
	{
		_phase = (_phase - 1) & 0x7;
		SetOutput();
	}

	static void StepBack()
	{
		_phase = (_phase - 2) & 0x6;
		SetOutput();
	}

	static void StepFwd()
	{
		_phase = (_phase + 2) & 0x6;
		SetOutput();
	}

protected:
	static void SetOutput()
	{
		static uint8_t aTable[] PROGMEM = {0x3f, 	0x0f, 0, 	0x0e, 0x3e, 0x0e, 0, 	0x0f};
		static uint8_t bTable[] PROGMEM = {0, 		0x0f, 0x3f, 0x0f, 0, 	0x0e, 0x3e, 0x0e};

		uint8_t a = pgm_read_byte(&(aTable[_phase]));
		uint8_t b = pgm_read_byte(&(bTable[_phase]));

		Base::Write(a, b);
	}
	static uint8_t _phase;
};

template <class DATA_PIN, class CLK_PIN, class SET_PIN, class ENABLE_PIN>
	uint8_t LB1946<DATA_PIN, CLK_PIN, SET_PIN, ENABLE_PIN>::_phase=0;



template<class IN1, class IN2, class E1, class IN3, class IN4, class E2>
class SimpleStepper //L293 driver, for example.
{
public:

	static void HalfStepFwd()
	{
		_phase = (_phase + 1) & 0x7;
		SetOutput();
	}

	static void HalfStepBack()
	{
		_phase = (_phase - 1) & 0x7;
		SetOutput();
	}

	static void StepBack()
	{
		_phase = (_phase - 2) & 0x6;
		SetOutput();
	}

	static void StepFwd()
	{
		_phase = (_phase + 2) & 0x6;
		SetOutput();
	}

	static void Enable()
	{
		e1.SetDirWrite();
		e2.SetDirWrite();
		in1.SetDirWrite();
		in2.SetDirWrite();
		in3.SetDirWrite();
		in4.SetDirWrite();
		e1.Set();
		e2.Set();
	}

	static void Disable()
	{
		e1.Clear();
		e2.Clear();
	}

protected:
	void SetOutput()
	{
		switch(_phase)
		{
			case 0: case 1:
				in1.Set(); in2.Clear(); in3.Clear(); in4.Clear();
			break;

			case 2: case 3:
				in1.Clear(); in2.Clear(); in3.Set(); in4.Clear();
			break;

			case 4: case 5:
				in1.Clear(); in2.Set(); in3.Clear(); in4.Clear();
			break;

			case 6: case 6:
				in1.Clear(); in2.Clear(); in3.Clear(); in4.Set();
			break;
		}
	}
	static IN1 in1;
	static IN2 in2;
	static E1 e1;
	static IN3 in3;
	static IN4 in4;
	static E2 e2;
	static uint8_t _phase;
};

template<class IN1, class IN2, class E1, class IN3, class IN4, class E2>
	uint8_t SimpleStepper<IN1, IN2, E1, IN3, IN4,  E2>::_phase=0;

#endif
