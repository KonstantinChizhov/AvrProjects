#pragma once

#ifndef PORTS_HPP
#define PORTS_HPP
#include <avr/io.h>
#include "static_assert.h"

#if defined(PORTCFG_MPCMASK) //XMega family
#include "XMegaPorts.h"
#else
#include "AvrPorts.h"
#endif

class Pin
{
public:
	Pin(volatile unsigned char &port, uint8_t pin)
	:_port(port)
	{
		_pin = pin; 
	}

	void Set()const
	{
		_port |= (1 << _pin);
	}

	void Set(uint8_t val)const
	{
		if(val)
			Set();
		else Clear();
	}

	void Clear()const
	{
		_port &= (uint8_t)~(1 << _pin);
	}

	void Togle()const
	{
		_port ^= (1 << _pin);
	}

	void SetDirWrite()const
	{
		*(&_port - 1) |= (1 << _pin);
	}

	void SetDirRead()const
	{
		*(&_port - 1) &= (uint8_t)~(1 << _pin);
	}

	uint8_t IsSet()const
	{
		return (*(&_port - 2)) & (uint8_t)(1 << _pin);
	}

private:
	volatile unsigned char &_port; 
	uint8_t _pin;
}; 


// this class represents one pin in a IO port.
// It is fully static.
template<class PORT, uint8_t PIN>
class TPin
{
	BOOST_STATIC_ASSERT(PIN < PORT::Width);
public:
	typedef PORT Port;
	enum{Number = PIN};

	static void Set()
	{
		PORT::data() |= (1 << PIN);
	}

	static void Set(uint8_t val)
	{
		if(val)
			Set();
		else Clear();
	}

	static void SetDir(uint8_t val)
	{
		if(val)
			SetDirWrite();
		else SetDirRead();
	}

	static void Clear()
	{
		PORT::data() &= (uint8_t)~(1 << PIN);
	}

	static void Togle()
	{
		PORT::data() ^= (1 << PIN);
	}

	static void SetDirRead()
	{
		PORT::dir() &= (uint8_t)~(1 << PIN);
	}

	static void SetDirWrite()
	{
		PORT::dir() |= (1 << PIN);
	}

	static uint8_t IsSet()
	{
		return PORT::pin() & (uint8_t)(1 << PIN);
	}	
	

	static void WaiteForSet()
	{
		while(IsSet()==0){}
	}

	static void WaiteForClear()
	{
		while(IsSet()){}
	}
}; 
//Short pin definations 
#ifdef PORTA
typedef TPin<Porta, 0> Pa0;
typedef TPin<Porta, 1> Pa1;
typedef TPin<Porta, 2> Pa2;
typedef TPin<Porta, 3> Pa3;
typedef TPin<Porta, 4> Pa4;
typedef TPin<Porta, 5> Pa5;
typedef TPin<Porta, 6> Pa6;
typedef TPin<Porta, 7> Pa7;
#endif

#ifdef PORTB
typedef TPin<Portb, 0> Pb0;
typedef TPin<Portb, 1> Pb1;
typedef TPin<Portb, 2> Pb2;
typedef TPin<Portb, 3> Pb3;
typedef TPin<Portb, 4> Pb4;
typedef TPin<Portb, 5> Pb5;
typedef TPin<Portb, 6> Pb6;
typedef TPin<Portb, 7> Pb7;
#endif

#ifdef PORTC
typedef TPin<Portc, 0> Pc0;
typedef TPin<Portc, 1> Pc1;
typedef TPin<Portc, 2> Pc2;
typedef TPin<Portc, 3> Pc3;
typedef TPin<Portc, 4> Pc4;
typedef TPin<Portc, 5> Pc5;
typedef TPin<Portc, 6> Pc6;
typedef TPin<Portc, 7> Pc7;
#endif

#ifdef PORTD
typedef TPin<Portd, 0> Pd0;
typedef TPin<Portd, 1> Pd1;
typedef TPin<Portd, 2> Pd2;
typedef TPin<Portd, 3> Pd3;
typedef TPin<Portd, 4> Pd4;
typedef TPin<Portd, 5> Pd5;
typedef TPin<Portd, 6> Pd6;
typedef TPin<Portd, 7> Pd7;
#endif

#ifdef PORTE
typedef TPin<Porte, 0> Pe0;
typedef TPin<Porte, 1> Pe1;
typedef TPin<Porte, 2> Pe2;
typedef TPin<Porte, 3> Pe3;
typedef TPin<Porte, 4> Pe4;
typedef TPin<Porte, 5> Pe5;
typedef TPin<Porte, 6> Pe6;
typedef TPin<Porte, 7> Pe7;
#endif

#ifdef PORTF
typedef TPin<Portf, 0> Pf0;
typedef TPin<Portf, 1> Pf1;
typedef TPin<Portf, 2> Pf2;
typedef TPin<Portf, 3> Pf3;
typedef TPin<Portf, 4> Pf4;
typedef TPin<Portf, 5> Pf5;
typedef TPin<Portf, 6> Pf6;
typedef TPin<Portf, 7> Pf7;
#endif

#ifdef PORTG
typedef TPin<Portg, 0> Pg0;
typedef TPin<Portg, 1> Pg1;
typedef TPin<Portg, 2> Pg2;
typedef TPin<Portg, 3> Pg3;
typedef TPin<Portg, 4> Pg4;
typedef TPin<Portg, 5> Pg5;
typedef TPin<Portg, 6> Pg6;
typedef TPin<Portg, 7> Pg7;
#endif

// TypeList and corresponding stuff are borrowed from Loki library
// http://sourceforge.net/projects/loki-lib/

// class template NullType - marks the end of type list
class NullType{};

////////////////////////////////////////////////////////////////////////////////
// class template Typelist
// The building block of typelists of any length
// Use it through the LOKI_TYPELIST_NN macros
// Defines nested types:
//     Head (first element, a non-typelist type by convention)
//     Tail (second element, can be another typelist)
////////////////////////////////////////////////////////////////////////////////

    template <class T, class U>
    struct Typelist
    {
       typedef T Head;
       typedef U Tail;
    };

////////////////////////////////////////////////////////////////////////////////
// class template Length
// Computes the length of a typelist
// Invocation (TList is a typelist):
// Length<TList>::value
// returns a compile-time constant containing the length of TList, not counting
//     the end terminator (which by convention is NullType)
////////////////////////////////////////////////////////////////////////////////

        template <class TList> struct Length;
        template <> struct Length<NullType>
        {
            enum { value = 0 };
        };
        
        template <class T, class U>
        struct Length< Typelist<T, U> >
        {
            enum { value = 1 + Length<U>::value };
        };
////////////////////////////////////////////////////////////////////////////////
// class template Erase
// Erases the first occurence, if any, of a type in a typelist
// Invocation (TList is a typelist and T is a type):
// Erase<TList, T>::Result
// returns a typelist that is TList without the first occurence of T
////////////////////////////////////////////////////////////////////////////////

        template <class TList, class T> struct Erase;
        
        template <class T>                         // Specialization 1
        struct Erase<NullType, T>
        {
            typedef NullType Result;
        };

        template <class T, class Tail>             // Specialization 2
        struct Erase<Typelist<T, Tail>, T>
        {
            typedef Tail Result;
        };

        template <class Head, class Tail, class T> // Specialization 3
        struct Erase<Typelist<Head, Tail>, T>
        {
            typedef Typelist<Head, 
                    typename Erase<Tail, T>::Result>
                Result;
        };
////////////////////////////////////////////////////////////////////////////////
// class template NoDuplicates
// Removes all duplicate types in a typelist
// Invocation (TList is a typelist):
// NoDuplicates<TList, T>::Result
////////////////////////////////////////////////////////////////////////////////

        template <class TList> struct NoDuplicates;
        
        template <> struct NoDuplicates<NullType>
        {
            typedef NullType Result;
        };

        template <class Head, class Tail>
        struct NoDuplicates< Typelist<Head, Tail> >
        {
        private:
            typedef typename NoDuplicates<Tail>::Result L1;
            typedef typename Erase<L1, Head>::Result L2;
        public:
            typedef Typelist<Head, L2> Result;
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

 		template <class TList, class T> struct GetPinsWithPort;

        template <class T>
        struct GetPinsWithPort<NullType, T>
        {
            typedef NullType Result;
        };

        template <class T, class Tail, uint8_t N, uint8_t M>
        struct GetPinsWithPort<Typelist<PW<TPin<T, N>, M>, Tail>, T>
        {
            // Go all the way down the list removing the type
           typedef Typelist<PW<TPin<T, N>, M>, 
                    typename GetPinsWithPort<Tail, T>::Result>
                Result;
        };

        template <class Head, class Tail, class T>
        struct GetPinsWithPort<Typelist<Head, Tail>, T>
        {
            // Go all the way down the list removing the type
			 typedef typename GetPinsWithPort<Tail, T>::Result Result;
        };

////////////////////////////////////////////////////////////////////////////////
// class template GetMask
// Computes bit mask for pin list
// Assume that TList is type list of PW (pin wrapper) types.
////////////////////////////////////////////////////////////////////////////////

		template <class TList> struct GetMask;
        template <> struct GetMask<NullType>
        {
            enum{value = 0};
        };
        template <class Head, class Tail>
        struct GetMask< Typelist<Head, Tail> >
        {
			enum{value = (1 << Head::Pin::Number) | GetMask<Tail>::value};
        };
		
////////////////////////////////////////////////////////////////////////////////
// class template PinWriteIterator
// Iterates througth pin list to compute value to write to their port
// Assume that TList is type list of PW (pin wrapper) types.
////////////////////////////////////////////////////////////////////////////////

		template <class TList> struct PinWriteIterator;
        template <> struct PinWriteIterator<NullType>
        {
			template<class DATA_T>
			static void UppendValue(DATA_T value, uint8_t &result)
			{   }

			template<class DATA_T>
			static inline void UppendReadValue(DATA_T &value, uint8_t portValue)
			{	}

        };
        template <class Head, class Tail>
        struct PinWriteIterator< Typelist<Head, Tail> >
        {
			template<class DATA_T>
			static inline void UppendValue(DATA_T value, uint8_t &result)
			{
				if((int)Head::Position == (int)Head::Pin::Number)
					result |= value & (1 << Head::Position);
				else 
					if(value & (1 << Head::Position))
						result |= (1 << Head::Pin::Number);

				PinWriteIterator<Tail>::UppendValue(value, result);
			}

			template<class DATA_T>
			static inline void UppendReadValue(DATA_T &value, uint8_t portValue)
			{
				if((int)Head::Position == (int)Head::Pin::Number)
					value |= portValue & (1 << Head::Position);

				else 
					if(portValue & (1 << Head::Pin::Number))
						value |= (1 << Head::Position);

				PinWriteIterator<Tail>::UppendReadValue(value, portValue);
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
			template<class DATA_T>
			static void Write(DATA_T value)
			{   }

			template<class DATA_T>
			static void Set(DATA_T value)
			{   }

			template<class DATA_T>
			static void Clear(DATA_T value)
			{   }

			template<class DATA_T>
			static void Read(DATA_T &value)
			{   }
			
			template<class DATA_T>
			static void DirWrite(DATA_T value)
			{	}

			template<class DATA_T>
			static void DirSet(DATA_T value)
			{   }

			template<class DATA_T>
			static void DirClear(DATA_T value)
			{   }

        };

        template <class Head, class Tail, class PinList>
        struct PortWriteIterator< Typelist<Head, Tail>, PinList>
        {
			//pins on current port
			typedef typename GetPinsWithPort<PinList, Head>::Result Pins;
			enum{Mask = GetMask<Pins>::value};
			typedef Head Port; //Head points to current port i the list.
			
			template<class DATA_T>
			static void Write(DATA_T value)
			{   
				if((int)Length<Pins>::value == (int)Port::Width)// whole port
				{
					uint8_t result=0;
					PinWriteIterator<Pins>::UppendValue(value, result);
					Port::Write(result);
				}
				else
				{
					uint8_t result=0;
					PinWriteIterator<Pins>::UppendValue(value, result);
					Port::Write((Port::Read() & ~Mask) | result) ;
				}
				PortWriteIterator<Tail, PinList>::Write(value);
			}

			template<class DATA_T>
			static void Set(DATA_T value)
			{   
				uint8_t result=0;
				PinWriteIterator<Pins>::UppendValue(value, result);
				Port::Set(result);
				
				PortWriteIterator<Tail, PinList>::Set(value);
			}

			template<class DATA_T>
			static void Clear(DATA_T value)
			{   
				uint8_t result=0;
				PinWriteIterator<Pins>::UppendValue(value, result);
				Port::Clear(result);
				
				PortWriteIterator<Tail, PinList>::Clear(value);
			}

			template<class DATA_T>
			static void DirWrite(DATA_T value)
			{   
				if((int)Length<Pins>::value == (int)Port::Width)
				{
					uint8_t result=0;
					PinWriteIterator<Pins>::UppendValue(value, result);
					Port::dir() = result;
				}
				else
				{
					uint8_t result=0;
					PinWriteIterator<Pins>::UppendValue(value, result);
					Port::DirWrite((Port::DirRead() & ~Mask) | result);
				}
				PortWriteIterator<Tail, PinList>::DirWrite(value);
			}

			template<class DATA_T>
			static void DirSet(DATA_T value)
			{   
				uint8_t result = 0;
				PinWriteIterator<Pins>::UppendValue(value, result);
				Port::DirSet(result);
				
				PortWriteIterator<Tail, PinList>::DirSet(value);
			}

			template<class DATA_T>
			static void DirClear(DATA_T value)
			{   
				uint8_t result=0;
				PinWriteIterator<Pins>::UppendValue(value, result);
				Port::DirClear(result);
				
				PortWriteIterator<Tail, PinList>::DirClear(value);
			}
			
			template<class DATA_T>
			static void Read(DATA_T &value)
			{   
				value = DATA_T(0);
				uint8_t portValue = Port::PinRead();
				PinWriteIterator<Pins>::UppendReadValue(value, portValue);
				
				PortWriteIterator<Tail, PinList>::Read(value);
			}

        };

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
			typedef typename GetPorts<PINS>::Result PinsToPorts;
			typedef typename NoDuplicates<PinsToPorts>::Result Ports; 
		public:
			enum{Length = Length<PINS>::value};

			template<class DATA_T>
			static void Write(DATA_T value)
			{
				PortWriteIterator<Ports, PINS>::Write(value);
			}

			template<class DATA_T>
			static void Set(DATA_T value)
			{
				PortWriteIterator<Ports, PINS>::Set(value);
			}

			template<class DATA_T>
			static void Clear(DATA_T value)
			{
				PortWriteIterator<Ports, PINS>::Clear(value);
			}

			template<class DATA_T>
			static void Read(DATA_T &value)
			{
				PortWriteIterator<Ports, PINS>::Read(value);
			}

			template<class DATA_T>
			static void DirWrite(DATA_T value)
			{
				PortWriteIterator<Ports, PINS>::DirWrite(value);
			}
			
			template<class DATA_T>
			static void DirSet(DATA_T value)
			{
				PortWriteIterator<Ports, PINS>::DirSet(value);
			}

			template<class DATA_T>
			static void DirClear(DATA_T value)
			{
				PortWriteIterator<Ports, PINS>::DirClear(value);
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
            typename T16 = NullType, typename T17 = NullType
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
                T17
            >
            ::Result TailResult;
			enum{PositionInList = Position};
        public:
            typedef Typelist< PW<T1, PositionInList>, TailResult> Result;

        };

        template<int Position>
        struct MakePinList<Position>
        {
            typedef NullType Result;
        };

////////////////////////////////////////////////////////////////////////////////
// class template PinList
// Represents generic set of IO pins htet could be used like a separete port.
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
            typename T16 = NullType, typename T17 = NullType
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
                	T17
				>::Result
			>
        {	};


#endif
