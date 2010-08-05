#pragma once
#ifndef LATCH_H
#define LATCH_H


	template<uint8_t StartBit, uint8_t Size>
	struct BitMask
	{
		
	};

	template<class Derived, class T>
	class LatchBase
	{
		protected:
			LatchBase()
			{
				_currentValue = 0;
			}
			void Write(T value)
			{
				Derived::Write(value);
			}

			T _currentValue;
		public:
			static void Set(T value)
			{
				Write(_currentValue |= value);
			}

			static void Clear(T value)
			{
				Write(_currentValue &= ~value);
			}

			static void Togle(T value)
			{
				Write(_currentValue ^= value);
			}

			template<uint8_t PIN>
			class Pin
			{
				typedef Derived Port;
				enum{Number = PIN};

				static void Set()
				{
					Port::Set(1 << PIN);
				}

				static void Set(uint8_t val)
				{
					if(val)
						Set();
					else Clear();
				}

				static void Clear()
				{
					Port::Clear(1 << PIN);
				}

				static void Togle()
				{
					Port::Togle(1 << PIN);
				}
			};

			template<uint8_t StartIndex, uint8_t Size>
			class Slice
			{
				public:
					static void Set(T value)
					{
						Write(_currentValue |= value);
					}

					static void Clear(T value)
					{
						Write(_currentValue &= ~value);
					}

					static void Togle(T value)
					{
						Write(_currentValue ^= value);
					}
			};
	};

	template<class ClockPin, class DataPin, class ResetPin, class T = uint8_t>
	class ThreePinLatch : public LatchBase<ThreePinLatch<ClockPin, DataPin, ResetPin, T>, T >
	{
		public:
		enum {BitsNumber = sizeof(T)*8};

		void Write(T value)
		{
			ResetPin::Set();
			ResetPin::Clear();			
			for(uint8_t i=0; i < BitsNumber; ++i)
			{
				DataPin::Set(value & 1);
				ClockPin::Set();
				value >>= 1;
				ClockPin::Clear();
			}
		}
	};
#endif
