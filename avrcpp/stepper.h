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
		CLK_PIN::Clear();
		SET_PIN::Clear();
		_delay_us(1);

		Write6(a);
		Write6(b);
		
		SET_PIN::Set();
		_delay_us(5);
		SET_PIN::Clear();
		_delay_us(_tD);
	}

	static void Enable()
	{
		ENABLE_PIN::Set();
	}

	static void Disable()
	{
		ENABLE_PIN::Clear();
	}

protected:

	static const int _tD = 2;

	static void Init()
	{
		ENABLE_PIN::Clear();
	
		DATA_PIN::SetDirWrite();
		SET_PIN::SetDirWrite();
		ENABLE_PIN::SetDirWrite();
		CLK_PIN::SetDirWrite();
	}

	static void Write6(uint8_t v)
	{
		for(uint8_t i=0; i < 6; ++i, v <<= 1)
		{
			DATA_PIN::Set((v & 0x20) != 0);
			CLK_PIN::Set();
			_delay_us(_tD);
			CLK_PIN::Clear();
			_delay_us(_tD);
		}
	}
};


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
class SimpleStepper //L293 driver, for example::
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
		E1::SetDirWrite();
		E2::SetDirWrite();
		IN1::SetDirWrite();
		IN2::SetDirWrite();
		IN3::SetDirWrite();
		IN4::SetDirWrite();
		E1::Set();
		E2::Set();
	}

	static void Disable()
	{
		E1::Clear();
		E2::Clear();
	}

protected:
	static void SetOutput()
	{
		switch(_phase)
		{
			case 0: case 1:
				IN1::Set(); IN2::Clear(); IN3::Clear(); IN4::Clear();
			break;

			case 2: case 3:
				IN1::Clear(); IN2::Clear(); IN3::Set(); IN4::Clear();
			break;

			case 4: case 5:
				IN1::Clear(); IN2::Set(); IN3::Clear(); IN4::Clear();
			break;

			case 6: case 6:
				IN1::Clear(); IN2::Clear(); IN3::Clear(); IN4::Set();
			break;
		}
	}

	static uint8_t _phase;
};

template<class IN1, class IN2, class E1, class IN3, class IN4, class E2>
	uint8_t SimpleStepper<IN1, IN2, E1, IN3, IN4,  E2>::_phase=0;

#endif
