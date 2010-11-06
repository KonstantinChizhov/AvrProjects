#pragma once

template<class Timer, class DataT, unsigned ThresholdArg>
class IrReciver
{
public:
	enum {Threshold = ThresholdArg};
	static void TimeoutHandler()
	{
		_code = _tempCode;
		_tempCode = 0;
		_rxComplete = 1;
	}

	static void CaptureHandler()
	{
		typename Timer::DataT delta;
		if(!_rxComplete)
		{
			delta = Timer::Get() - _prevValue;
			_tempCode <<= 1;
			if(delta > Threshold) 
				_tempCode |= 1;
		}
		else{
			_rxComplete = 0;
		}
		_prevValue = Timer::Get();
	}

	DataT Code(){return _code;}
	
private:
	static uint8_t _rxComplete;
	static typename Timer::DataT _prevValue;
	static DataT _code;
	static DataT _tempCode;
};


template<class Timer, class DataT, unsigned ThresholdArg>
uint8_t IrReciver<Timer, DataT, ThresholdArg>::_rxComplete;

template<class Timer, class DataT, unsigned ThresholdArg>
typename Timer::DataT IrReciver<Timer, DataT, ThresholdArg>::_prevValue;

template<class Timer, class DataT, unsigned ThresholdArg>
DataT IrReciver<Timer, DataT, ThresholdArg>::_code;

template<class Timer, class DataT, unsigned ThresholdArg>
DataT IrReciver<Timer, DataT, ThresholdArg>::_tempCode;
