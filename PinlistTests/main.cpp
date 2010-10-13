#include <iostream>
#include <string>
#include "iopins.h"
#include "pinlist.h"

using namespace std;
using namespace IO;
using namespace IO::Test;

typedef TestPort<unsigned, 'A'> Porta;
typedef TestPort<unsigned, 'B'> Portb;

DECLARE_PORT_PINS(Porta, Pa)

DECLARE_PORT_PINS(Portb, Pb)

#define ASSERT_EQUAL(value, expected) if((value) != (expected)){\
    std::cout << "Assertion failed! "  << "\t File: " << __FILE__ << "\t function: " << __FUNCTION__ << "\t line: " << __LINE__ << std::endl;\
    std::cout << std::hex << "Expacted: 0x" << (unsigned)(expected) << "\tgot: 0x" << (unsigned)(value);\
    exit(1);\
    }


template<class Pins>
struct PrintPinList
{

    template<class List, int index>
    struct Iterator
    {
        static void Print()
        {
            Iterator<List, index-1>::Print();
            typedef typename List:: template Pin<index-1> CurrentPin;
            if(index == List::Length)
                std::cout << (char)CurrentPin::Port::Id << CurrentPin::Number;
            else
                std::cout << (char)CurrentPin::Port::Id << CurrentPin::Number << ", ";
        }
    };

    template<class List>
    struct Iterator<List, 0>
    {
        static void Print()
        {}
    };

    static void Print()
    {
        std::cout << "PinList<";
        Iterator<Pins, Pins::Length>::Print();
        std::cout << ">";
    }
};


template<class Pins>
void TestOnePortPinList(unsigned listValue, unsigned portValue)
{
    typedef typename Pins::template Pin<0>::Port Port;
    typename Pins::DataType val;
    cout << __FUNCTION__ << "\t";
    PrintPinList<Pins>::Print();

    Port::Write(0);

    Pins::Write(listValue);
    ASSERT_EQUAL(Port::Out,  portValue);
    val = Pins::Read();
    ASSERT_EQUAL(val, listValue);

    Pins::DirWrite(listValue);
    ASSERT_EQUAL(Port::Dir, portValue);

    Port::Write(0);
    Port::DirWrite(0);

    Port::In = portValue;
    val = Pins::PinRead();
    ASSERT_EQUAL(val, listValue);

    Port::In = 0;
    val = Pins::PinRead();
    ASSERT_EQUAL(val, 0);

    Pins::Write(0);
    ASSERT_EQUAL(Port::Out, 0);

    Pins::Set(listValue);
    ASSERT_EQUAL(Port::Out, portValue);

    Pins::Clear(listValue);
    ASSERT_EQUAL(Port::Out, 0);

    Pins::DirWrite(0);
    ASSERT_EQUAL(Port::Dir, 0);

    Pins::DirSet(listValue);
    ASSERT_EQUAL(Port::Dir, portValue);

    Pins::DirClear(listValue);
    ASSERT_EQUAL(Port::Dir, 0);

    cout << "\tOK" << endl;
}


int main()
{
    for(int i=0; i< 16; i++)
    {
        cout << "Writing value: " << i << endl;
        TestOnePortPinList<PinList<Pa0, Pa1, Pa2, Pa3> >(i, i);
        TestOnePortPinList<PinList<Pa1, Pa2, Pa3, Pa4> >(i, i << 1);
        TestOnePortPinList<PinList<Pa2, Pa3, Pa4, Pa5> >(i, i << 2);
    }

    TestOnePortPinList<PinList<Pa1, Pa3, Pa2, Pa0> >(0x0f, 0x0f);
    TestOnePortPinList<PinList<Pa0, Pa2, Pa1, Pa3> >(0x0f, 0x0f);
    TestOnePortPinList<PinList<Pa2, Pa1, Pa3, Pa4, Pa6> >(0x1f, 0x5e);

    TestOnePortPinList<PinList<Pa5, Pa6, Pa7, Pa0, Pa1, Pa2, Pa3, Pa4> >(0xff, 0xff);

    TestOnePortPinList<PinList<Pa2, Pa1, Pa3, Pa4, Pa6, Pa8, Pa7, Pa0, Pa5> >(0x1ff, 0x1ff);

    TestOnePortPinList<PinList<Pa0, Pa1, Pa2, Pa3, Pa4, Pa5, Pa6, Pa7, Pa8>::Slice<5, 4> >(0x1e0, 0x1e0);
    TestOnePortPinList<PinList<Pa0, Pa1, Pa2, Pa3, Pa4, Pa5, Pa6, Pa7, Pa8>::Slice<0, 4> >(0x0f, 0x0f);

    return 0;
}
