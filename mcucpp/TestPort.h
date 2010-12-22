//*****************************************************************************
//
// Title		: C++ IO Test port for PinList
// Author		: Konstantin Chizhov
// Date			: 2010
//
//       This code is distributed under the GNU Public License
//       which can be found at http://www.gnu.org/licenses/gpl.txt
//
// As a special exception, you may use this file as part of a free software
// library without restriction. Specifically, if other files instantiate
// templates or use macros or inline functions from this file, or you compile
// this file and link it with other files to produce an executable, this
// file does not by itself cause the resulting executable to be covered by
// the GNU General Public License. This exception does not however
// invalidate any other reasons why the executable file might be covered by
// the GNU General Public License.
//*****************************************************************************

namespace IO
{
	namespace Test
	{
		template<class DataType, unsigned Identity>
		class TestPort
		{
			public:
			enum Configuration
			{
				AnalogIn = 0,
				In = 0x00,
				PullUpOrDownIn = 0x00,
				Out = 0x01,
				AltOut = 0x01,
			};
		
			template<unsigned pin, >
			static void SetPinConfiguration(Configuration configuration)
			{
				BOOST_STATIC_ASSERT(pin < Width);
				if(configuration)
					Dir |= 1 << pin;
				else
					Dir &= ~(1 << pin);
			}
			
			static void SetConfiguration(DataT mask, Configuration configuration)
			{
				if(configuration)
					Dir |= mask;
				else
					Dir &= ~mask;
			}
			
			typedef DataType DataT;
			static void Write(DataT value)
			{
				Out = value;
			}
			static void ClearAndSet(DataT clearMask, DataT value)
			{
				Out &= ~clearMask;
				Out |= value;
			}
			static DataT Read()
			{
				return Out;
			}
			static void Set(DataT value)
			{
				Out |= value;
			}
			static void Clear(DataT value)
			{
				Out &= ~value;
			}
			static void Togle(DataT value)
			{
				Out ^= value;
			}
			static DataT PinRead()
			{
				return In;
			}
			enum{Id = Identity};
			enum{Width=sizeof(DataT)*8};

			volatile static DataType Out;
			volatile static DataType Dir;
			volatile static DataType In;
		};

		template<class DataType, unsigned Identity>
		volatile DataType TestPort<DataType, Identity>::Out;

		template<class DataType, unsigned Identity>
		volatile DataType TestPort<DataType, Identity>::Dir;

		template<class DataType, unsigned Identity>
		volatile DataType TestPort<DataType, Identity>::In;
	}
}
