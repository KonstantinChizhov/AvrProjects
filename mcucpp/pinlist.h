

//*****************************************************************************
//
// Title		: C++ IO for ATMEL AVR
// Author		: Konstantin Chizhov
// Date			: 2010
// Target MCU	: paltform indepandent
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

#pragma once

#include "iopin.h"
#include "loki\Typelist.h"

namespace IO
{
	using namespace Loki;
	using namespace Loki::TL;

	namespace IoPrivate
	{
	
		template<bool condition, class TypeIfTrue, class TypeIfFale>
		struct StaticIf
		{
			typedef TypeIfTrue Result;
		};

		template<class TypeIfTrue, class TypeIfFale>
		struct StaticIf<false, TypeIfTrue, TypeIfFale>
		{
			 typedef TypeIfFale Result;
		};

		template<unsigned sizeBits>
		struct SelectSize
		{
			enum{LessOrEq8 = sizeBits <= 8, LessOrEq16 = sizeBits <= 16};
			typedef typename StaticIf<
					LessOrEq8,
					uint8_t,
					typename StaticIf<LessOrEq16, uint16_t, uint32_t>::Result>
					::Result Result;
		};


		template<unsigned BitsToShift>
		struct ShiftLeft
		{
			template<class T>
			static T Shift(T value)
			{
				return value << BitsToShift;
			}
		};

		template<unsigned BitsToShift>
		struct ShiftRight
		{
			template<class T>
			static T Shift(T value)
			{
				return value >> BitsToShift;
			}
		};

		enum MapDirection{ValueToPort = 0 , PortToValue = 1};

		template<unsigned First, unsigned Second, MapDirection mapDirection>
		class Shifter
		{
			typedef ShiftRight<First - Second> RightShifter1;
			typedef ShiftRight<Second - First> RightShifter2;

			typedef ShiftLeft<First - Second> LeftShifter1;
			typedef ShiftLeft<Second - First> LeftShifter2;

			typedef typename StaticIf<mapDirection, LeftShifter1, RightShifter1>::Result FirstShifter;
			typedef typename StaticIf<mapDirection, LeftShifter2, RightShifter2>::Result SecondShifter;

			enum{ShiftDirection = First > Second};
			typedef typename StaticIf<ShiftDirection, FirstShifter, SecondShifter>::Result ActualShifter;
		public:
			template<class T>
			static T Shift(T value)
			{
				return ActualShifter::Shift(value);
			}
		};
		
		template<class PINS> struct GetLastBitPosition;

		template<>
		struct GetLastBitPosition<NullType>
		{
		    enum {value = 0};
		};

		template<class Head>
		struct GetLastBitPosition<Typelist<Head, NullType> >
		{
		    enum {value = Head::Position};
		};

		template<class Head, class Tail>
		struct GetLastBitPosition<Typelist<Head, Tail> >
		{
		    enum {value = GetLastBitPosition<Tail>::value};
		};
	
////////////////////////////////////////////////////////////////////////////////
// class template PW. Pin wrapper
// Holds a Pin class and its bit position in value to read/write
////////////////////////////////////////////////////////////////////////////////

		template<class TPIN, uint8_t POSITION>
		struct PW	//Pin wrapper
		{
			typedef TPIN Pin;
			enum{Position = POSITION};
		};

////////////////////////////////////////////////////////////////////////////////
// class template GetPorts
// Converts list of Pin types to Port types
////////////////////////////////////////////////////////////////////////////////

 		template <class TList> struct GetPorts;

        template <> struct GetPorts<NullType>
        {
            typedef NullType Result;
        };

        template <class Head, class Tail>
        struct GetPorts< Typelist<Head, Tail> >
        {
        private:
			typedef typename Head::Pin::Port Port;
            typedef typename GetPorts<Tail>::Result L1;
        public:
            typedef Typelist<Port, L1> Result;
        };

////////////////////////////////////////////////////////////////////////////////
// class template GetPinsWithPort
// Selects pins types form pin list which have givven port
// Assume that TList is type list of PW (pin wrapper) types.
// T - port type to select by.
////////////////////////////////////////////////////////////////////////////////

 		template <class TList, class TPort> struct GetPinsWithPort;

        template <class TPort>
        struct GetPinsWithPort<NullType, TPort>
        {
            typedef NullType Result;
        };

        template <class TPort, class Tail, uint8_t PortBitPosition, uint8_t ValueBitPosition>
        struct GetPinsWithPort<Typelist<PW<TPin<TPort, PortBitPosition>, ValueBitPosition>, Tail>, TPort>
        {
            // Go all the way down the list removing the type
           typedef Typelist<PW<TPin<TPort, PortBitPosition>, ValueBitPosition>,
                    typename GetPinsWithPort<Tail, TPort>::Result>
                Result;
        };

        template <class Head, class Tail, class TPort>
        struct GetPinsWithPort<Typelist<Head, Tail>, TPort>
        {
            // Go all the way down the list removing the type
			 typedef typename GetPinsWithPort<Tail, TPort>::Result Result;
        };

////////////////////////////////////////////////////////////////////////////////
// class template GetPortMask
// Computes port bit mask for pin list
// Assume that TList is type list of PW (pin wrapper) types.
////////////////////////////////////////////////////////////////////////////////

		template <class TList> struct GetPortMask;

        template <> struct GetPortMask<NullType>
        {
            enum{value = 0};
        };

        template <class Head, class Tail>
        struct GetPortMask< Typelist<Head, Tail> >
        {
			enum{value = (1 << Head::Pin::Number) | GetPortMask<Tail>::value};
        };

////////////////////////////////////////////////////////////////////////////////
// class template GetValueMask
// Computes value bit mask for pin list
// Assume that TList is type list of PW (pin wrapper) types.
////////////////////////////////////////////////////////////////////////////////

		template <class TList> struct GetValueMask;

        template <> struct GetValueMask<NullType>
        {
            enum{value = 0};
        };

        template <class Head, class Tail>
        struct GetValueMask< Typelist<Head, Tail> >
        {
			enum{value = (1 << Head::Position) | GetValueMask<Tail>::value};
        };
////////////////////////////////////////////////////////////////////////////////
// class template IsSerial
// Computes if pin list is seqental
// Assume that TList is type list of PW (pin wrapper) types.
////////////////////////////////////////////////////////////////////////////////

		template <class TList> struct IsSerial;

        template <> struct IsSerial<NullType>
        {
            enum{value = 1};
			enum{PinNumber = -1};
			enum{EndOfList = 1};
        };
        template <class Head, class Tail>

        struct IsSerial< Typelist<Head, Tail> >
        {
		//private:
			typedef IsSerial<Tail> I;
			enum{PinNumber = Head::Pin::Number};
			enum{EndOfList = 0};
		//public:
			enum{value = ((PinNumber == I::PinNumber - 1) && I::value) || I::EndOfList};
        };
////////////////////////////////////////////////////////////////////////////////
// Returns first Num elements from Typelist
////////////////////////////////////////////////////////////////////////////////
		template <class TList, uint8_t Num> struct TakeFirst;

		template <>
		struct TakeFirst<NullType, 0>
		{
			typedef NullType Result;
		};

		template <class Head, class Tail>
		struct TakeFirst<Typelist<Head, Tail>, 0>
		{
			typedef NullType Result;
		};

		template <class Head, class Tail, uint8_t Num>
		struct TakeFirst<Typelist<Head, Tail>, Num>
		{
			typedef Typelist<Head,
						typename TakeFirst<Tail, Num-1>::Result
						>Result;
		};
////////////////////////////////////////////////////////////////////////////////
// Skip Num first elements from tipelist
////////////////////////////////////////////////////////////////////////////////
		template <class TList, uint8_t Num> struct SkipFirst;

		template <>
		struct SkipFirst<NullType, 0>
		{
			typedef NullType Result;
		};

		template <class Head, class Tail>
		struct SkipFirst<Typelist<Head, Tail>, 0>
		{
			typedef Typelist<Head, Tail> Result;
		};

		template <class Head, class Tail, uint8_t Num>
		struct SkipFirst<Typelist<Head, Tail>, Num>
		{
			typedef typename SkipFirst<Tail, Num-1>::Result Result;
		};

////////////////////////////////////////////////////////////////////////////////
// class template PinWriteIterator
// Iterates througth pin list to compute value to write to their port
// Assume that TList is type list of PW (pin wrapper) types.
////////////////////////////////////////////////////////////////////////////////

		template <class TList> struct PinWriteIterator;

        template <> struct PinWriteIterator<NullType>
        {
			typedef uint8_t PortDataType;

			template<class DataType>
			static PortDataType UppendValue(DataType value)
			{
				return 0;
			}

			template<class DataType>
			static inline DataType UppendReadValue(PortDataType portValue, DataType)
			{
				return 0;
			}

        };

        template <class Head, class Tail>
        struct PinWriteIterator< Typelist<Head, Tail> >
        {
			typedef typename Head::Pin::Port::DataT PortDataType;

			template<class DataType>
			static inline PortDataType UppendValue(DataType value)
			{
				using namespace IoPrivate;
				if(IsSerial<Typelist<Head, Tail> >::value)
				{
					return Shifter<
							Head::Pin::Number, 	//bit position in port
							Head::Position, 	//bit position in value
							ValueToPort>::Shift(value) &
						GetPortMask<Typelist<Head, Tail> >::value;
				}

				PortDataType result=0;

				if((int)Head::Position == (int)Head::Pin::Number)
					result |= value & (1 << Head::Position);
				else
					if(value & (1 << Head::Position))
						result |= (1 << Head::Pin::Number);

				return result | PinWriteIterator<Tail>::UppendValue(value);
			}

			template<class DataType>
			static inline DataType UppendReadValue(PortDataType portValue, DataType dummy)
			{
				using namespace IoPrivate;
				if(IsSerial<Typelist<Head, Tail> >::value)
				{
					typedef Shifter<
							Head::Pin::Number, 	//bit position in port
							Head::Position, 	//bit position in value
							PortToValue> AtctualShifter;

					return AtctualShifter::Shift(DataType(portValue)) &
						GetValueMask<Typelist<Head, Tail> >::value;
				}

				DataType value=0;
				if((int)Head::Position == (int)Head::Pin::Number)
					value |= portValue & (1 << Head::Position);
				else
					if(portValue & (1 << Head::Pin::Number))
						value |= (1 << Head::Position);

				return value | PinWriteIterator<Tail>::UppendReadValue(portValue, dummy);
			}
        };

////////////////////////////////////////////////////////////////////////////////
// class template PortWriteIterator
// Iterates througth port list and write value to them
// Assume that PinList is type list of PW (pin wrapper) types.
// And PortList is type list of port types.
////////////////////////////////////////////////////////////////////////////////

		template <class PortList, class PinList> struct PortWriteIterator;

        template <class PinList> struct PortWriteIterator<NullType, PinList>
        {
			template<class DataType>
			static void Write(DataType value)
			{   }

			template<class DataType>
			static void Set(DataType value)
			{   }

			template<class DataType>
			static void Clear(DataType value)
			{   }

			template<class DataType>
			static DataType PinRead(DataType)
			{
				return 0;
			}

			template<class DataType>
			static void DirWrite(DataType value)
			{	}

			template<class DataType>
			static void DirSet(DataType value)
			{   }

			template<class DataType>
			static void DirClear(DataType value)
			{   }

			template<class DataType>
			static DataType OutRead(DataType dummy)
			{
				return 0;
			}
        };

        template <class Head, class Tail, class PinList>
        struct PortWriteIterator< Typelist<Head, Tail>, PinList>
        {
			//pins on current port
			typedef typename GetPinsWithPort<PinList, Head>::Result Pins;
			enum{Mask = GetPortMask<Pins>::value};
			typedef Head Port; //Head points to current port i the list.

			template<class DataType>
			static void Write(DataType value)
			{
				DataType result = PinWriteIterator<Pins>::UppendValue(value);
				if((int)Length<Pins>::value == (int)Port::Width)// whole port
					Port::Write(result);
				else
				{
					Port::ClearAndSet(Mask, result);
					//Port::Clear(Mask);
					//Port::Set(result);
				}

				PortWriteIterator<Tail, PinList>::Write(value);
			}

			template<class DataType>
			static void Set(DataType value)
			{
				DataType result = PinWriteIterator<Pins>::UppendValue(value);
				Port::Set(result);

				PortWriteIterator<Tail, PinList>::Set(value);
			}

			template<class DataType>
			static void Clear(DataType value)
			{
				DataType result = PinWriteIterator<Pins>::UppendValue(value);
				Port::Clear(result);

				PortWriteIterator<Tail, PinList>::Clear(value);
			}

			template<class DataType>
			static void DirWrite(DataType value)
			{
				DataType result = PinWriteIterator<Pins>::UppendValue(value);
				if((int)Length<Pins>::value == (int)Port::Width)
					Port::DirWrite(result);
				else
				{
					Port::DirClearAndSet(Mask, result);
				}

				PortWriteIterator<Tail, PinList>::DirWrite(value);
			}

			template<class DataType>
			static void DirSet(DataType value)
			{
				DataType result = PinWriteIterator<Pins>::UppendValue(value);
				Port::DirSet(result);

				PortWriteIterator<Tail, PinList>::DirSet(value);
			}

			template<class DataType>
			static void DirClear(DataType value)
			{
				DataType result = PinWriteIterator<Pins>::UppendValue(value);
				Port::DirClear(result);

				PortWriteIterator<Tail, PinList>::DirClear(value);
			}

			template<class DataType>
			static DataType PinRead(DataType dummy)
			{
				typename Port::DataT portValue = Port::PinRead();
				DataType value = PinWriteIterator<Pins>::UppendReadValue(portValue, dummy);
				return value | PortWriteIterator<Tail, PinList>::PinRead(dummy);
			}

			template<class DataType>
			static DataType OutRead(DataType dummy)
			{
				typename Port::DataT portValue = Port::Read();
				DataType value = PinWriteIterator<Pins>::UppendReadValue(portValue, dummy);
				return value | PortWriteIterator<Tail, PinList>::OutRead(dummy);
			}

        };
	}
////////////////////////////////////////////////////////////////////////////////
// class template PinSet
// Holds implimentation of pin list manipulations.
// Pins from list are grouped by their port and group read/write operation is
// performed on each port.
////////////////////////////////////////////////////////////////////////////////

		template<class PINS>
		struct PinSet
		{
		private:
			typedef typename IoPrivate::GetPorts<PINS>::Result PinsToPorts;
		public:
			typedef PINS PinTypeList;
			typedef typename Loki::TL::NoDuplicates<PinsToPorts>::Result Ports;
			enum{Length = Length<PINS>::value};
			enum{LastBitPosition = IoPrivate::GetLastBitPosition<PINS>::value};

			typedef typename IoPrivate::SelectSize<LastBitPosition+1>::Result DataType;

			template<uint8_t Num>
			class Take: public PinSet< typename IoPrivate::TakeFirst<PINS, Num>::Result >
			{};

			template<uint8_t Num>
			class Skip: public PinSet< typename IoPrivate::SkipFirst<PINS, Num>::Result >
			{};

			template<uint8_t StartIndex, uint8_t Size>
			class Slice: public PinSet
					<
						typename IoPrivate::SkipFirst<
							typename IoPrivate::TakeFirst<PINS, StartIndex + Size>::Result,
							StartIndex>::Result
					>
			{
				BOOST_STATIC_ASSERT(StartIndex + Size <= Length);
			};

			template<uint8_t PIN>
			class Pin :public TypeAt<PINS, PIN>::Result::Pin
			{};

			static void Write(DataType value)
			{
				IoPrivate::PortWriteIterator<Ports, PINS>::Write(value);
			}

			static DataType Read()
			{
				typedef IoPrivate::PortWriteIterator<Ports, PINS> iter;
				return iter::OutRead(DataType(0));
			}
			static void Set(DataType value)
			{
				IoPrivate::PortWriteIterator<Ports, PINS>::Set(value);
			}

			static void Clear(DataType value)
			{
				IoPrivate::PortWriteIterator<Ports, PINS>::Clear(value);
			}

			static DataType PinRead()
			{
				typedef IoPrivate::PortWriteIterator<Ports, PINS> iter;
				return iter::PinRead(DataType(0));
			}

			static void DirWrite(DataType value)
			{
				IoPrivate::PortWriteIterator<Ports, PINS>::DirWrite(value);
			}

			static void DirSet(DataType value)
			{
				IoPrivate::PortWriteIterator<Ports, PINS>::DirSet(value);
			}

			static void DirClear(DataType value)
			{
				IoPrivate::PortWriteIterator<Ports, PINS>::DirClear(value);
			}
		};

////////////////////////////////////////////////////////////////////////////////
// class template MakePinList
// This class is used to generate PinList and associate each pin in the list with
// its bit position in value to be Write to and Read from pin list.
////////////////////////////////////////////////////////////////////////////////

		template
        <
			int Position,
            typename T1  = NullType, typename T2  = NullType, typename T3  = NullType,
            typename T4  = NullType, typename T5  = NullType, typename T6  = NullType,
            typename T7  = NullType, typename T8  = NullType, typename T9  = NullType,
            typename T10 = NullType, typename T11 = NullType, typename T12 = NullType,
            typename T13 = NullType, typename T14 = NullType, typename T15 = NullType,
            typename T16 = NullType, typename T17 = NullType, typename T18 = NullType,
			typename T19 = NullType, typename T20 = NullType, typename T21 = NullType,
			typename T22 = NullType, typename T23 = NullType, typename T24 = NullType,
			typename T25 = NullType, typename T26 = NullType, typename T27 = NullType,
			typename T28 = NullType, typename T29 = NullType, typename T30 = NullType,
			typename T31 = NullType, typename T32 = NullType, typename T33 = NullType
        >
        struct MakePinList
        {
        private:
            typedef typename MakePinList
            <
				Position + 1,
                T2 , T3 , T4 ,
                T5 , T6 , T7 ,
                T8 , T9 , T10,
                T11, T12, T13,
                T14, T15, T16,
                T17, T18, T19,
				T20, T21, T22,
				T23, T24, T25,
				T26, T27, T28,
				T29, T30, T31,
				T32, T33
            >
            ::Result TailResult;
			enum{PositionInList = Position};
        public:
            typedef Typelist< IoPrivate::PW<T1, PositionInList>, TailResult> Result;

        };

        template<int Position>
        struct MakePinList<Position>
        {
            typedef NullType Result;
        };

////////////////////////////////////////////////////////////////////////////////
// class template PinList
// Represents generic set of IO pins that could be used like a virtual port.
// It can be composed from any number of pins from 1 to 16 from any IO port present in selected device
// (the last T17 type in PinList is a end of list marker).
// It can be used like this:
//		typedef PinList<Pa0, Pa1, Pa2, Pa3, Pb5, Pb4, Pb2> pins;
//		pins::Write(someValue);

////////////////////////////////////////////////////////////////////////////////

		template
        <
            typename T1  = NullType, typename T2  = NullType, typename T3  = NullType,
            typename T4  = NullType, typename T5  = NullType, typename T6  = NullType,
            typename T7  = NullType, typename T8  = NullType, typename T9  = NullType,
            typename T10 = NullType, typename T11 = NullType, typename T12 = NullType,
            typename T13 = NullType, typename T14 = NullType, typename T15 = NullType,
            typename T16 = NullType, typename T17 = NullType, typename T18 = NullType,
			typename T19 = NullType, typename T20 = NullType, typename T21 = NullType,
			typename T22 = NullType, typename T23 = NullType, typename T24 = NullType,
			typename T25 = NullType, typename T26 = NullType, typename T27 = NullType,
			typename T28 = NullType, typename T29 = NullType, typename T30 = NullType,
			typename T31 = NullType, typename T32 = NullType, typename T33 = NullType
        >
        struct PinList: public PinSet
			<
				typename MakePinList
				<	0,	T1,
					T2 , T3 , T4 ,
                	T5 , T6 , T7 ,
                	T8 , T9 , T10,
                	T11, T12, T13,
                	T14, T15, T16,
                	T17, T18, T19,
					T20, T21, T22,
					T23, T24, T25,
					T26, T27, T28,
					T29, T30, T31,
					T32, T33
				>::Result
			>
        {	};

}
