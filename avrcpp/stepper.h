#pragma once
#ifndef STEP_MOTOR_HPP
#define STEP_MOTOR_HPP


template <class DATA_PIN, class CLK_PIN, class SET_PIN, class ENABLE_PIN>
class LB1946
{
public:
	LB1946()
	{
		Init();
	}

	static void StepFwd()
	{
		uint8_t a = 0, b = 0;
		switch(_phase++)
		{
			case 0:
				a = 0x3f; b = 0;
			break;
			case 1:
				a = 0; b = 0x3f;
			break;
			case 2:
				a = 0x3e; b = 0;
			break;
			case 3:
				a = 0; b = 0x3e;
			_phase = 0;
			break;		
		}
		Write(a, b);
	}

	static void StepBack()
	{
		uint8_t a = 0, b = 0;
		switch(_phase--)
		{
			case 0:
				a = 0x3f; b = 0;
				_phase = 3;
			break;
			case 1:
				a = 0; b = 0x3f;
			break;
			case 2:
				a = 0x3e; b = 0;
			break;
			case 3:
				a = 0; b = 0x3e;
			break;		
		}
		Write(a, b);
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
		_phase = 1;
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

	static uint8_t _phase;
	static DATA_PIN _data;
	static CLK_PIN _clk;
	static SET_PIN _set;
	static ENABLE_PIN _enable;
};

	template <class DATA_PIN, class CLK_PIN, class SET_PIN, class ENABLE_PIN>
	uint8_t LB1946<DATA_PIN, CLK_PIN, SET_PIN, ENABLE_PIN>::_phase;

	template <class DATA_PIN, class CLK_PIN, class SET_PIN, class ENABLE_PIN>
	DATA_PIN LB1946<DATA_PIN, CLK_PIN, SET_PIN, ENABLE_PIN>::_data;

	template <class DATA_PIN, class CLK_PIN, class SET_PIN, class ENABLE_PIN>
	CLK_PIN LB1946<DATA_PIN, CLK_PIN, SET_PIN, ENABLE_PIN>::_clk;

	template <class DATA_PIN, class CLK_PIN, class SET_PIN, class ENABLE_PIN>
	SET_PIN LB1946<DATA_PIN, CLK_PIN, SET_PIN, ENABLE_PIN>::_set;

	template <class DATA_PIN, class CLK_PIN, class SET_PIN, class ENABLE_PIN>
	ENABLE_PIN LB1946<DATA_PIN, CLK_PIN, SET_PIN, ENABLE_PIN>::_enable;



#endif
