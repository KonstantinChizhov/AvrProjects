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
    class NativePortBase
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
    };

	namespace Test
	{
		template<class DataType, unsigned Identity>
		class TestPort :public NativePortBase
		{
			public:

            typedef DataType DataT;

			template<unsigned pin>
			static void SetPinConfiguration(Configuration configuration)
			{
				BOOST_STATIC_ASSERT(pin < Width);
				if(configuration)
					DirReg |= 1 << pin;
				else
					DirReg &= ~(1 << pin);
			}

			static void SetConfiguration(DataT mask, Configuration configuration)
			{
				if(configuration)
					DirReg |= mask;
				else
					DirReg &= ~mask;
			}

			static void Write(DataT value)
			{
				OutReg = value;
			}
			static void ClearAndSet(DataT clearMask, DataT value)
			{
				OutReg &= ~clearMask;
				OutReg |= value;
			}
			static DataT Read()
			{
				return OutReg;
			}
			static void Set(DataT value)
			{
				OutReg |= value;
			}
			static void Clear(DataT value)
			{
				OutReg &= ~value;
			}
			static void Togle(DataT value)
			{
				OutReg ^= value;
			}
			static DataT PinRead()
			{
				return InReg;
			}
			enum{Id = Identity};
			enum{Width=sizeof(DataT)*8};

			volatile static DataType OutReg;
			volatile static DataType DirReg;
			volatile static DataType InReg;
		};

		template<class DataType, unsigned Identity>
		volatile DataType TestPort<DataType, Identity>::OutReg;

		template<class DataType, unsigned Identity>
		volatile DataType TestPort<DataType, Identity>::DirReg;

		template<class DataType, unsigned Identity>
		volatile DataType TestPort<DataType, Identity>::InReg;
	}
}
