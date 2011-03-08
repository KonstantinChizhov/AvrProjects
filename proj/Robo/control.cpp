#include <avr/io.h>
#include <iopins.h>
#include "protocol.h"
#include <pinlist.h>
#include <spi.h>
#include <delay.h>
#include <TextFormater.h>

using namespace IO;

typedef Pa5Inv Fwd;
typedef Pb5Inv Back;
typedef Pa3Inv Left;
typedef Pb6Inv Right;
typedef Pa1Inv Btn1;
typedef Pa7Inv Btn2;
typedef Pa2Inv Btn3;
typedef Pa6Inv Btn4;
typedef Pa0Inv Btn5;
typedef Pa4Inv Btn6;

typedef PinList<Fwd, Back, Left, Right, Btn1, Btn2, Btn3, Btn4, Btn5, Btn6> Buttons;


template<class TxPin, uint32_t Baund>
class SoftUsart
{
    static const uint16_t BitDelay = 1000000/ Baund;
    public:
    static void Putch(uint8_t value)
    {
        TxPin::SetConfiguration(TxPin::Port::Out);
        TxPin::Clear();
        Util::delay_us<BitDelay, F_CPU>();
        for(uint8_t i = 0; i < 8; i++)
        {
            TxPin::Set((value & 1) );
            value >>= 1;
            Util::delay_us<BitDelay, F_CPU>();
        }
        TxPin::Set();
        Util::delay_us<BitDelay*5, F_CPU>();
    }
};

typedef SoftUsart<Pa4, 9600> usart;
TextFormater< usart, 16 > debug;

#include <Rfm70.h>

typedef SoftSpi<Pb0, Pb1, Pb2> Spi;
typedef Rfm70<Spi, Pb4, Pb3, NullPin> Transiver;

uint16_t keys;

__attribute((OS_main))
int main()
{

	Transiver::Init();
	Transiver::SwitchToTxMode();
	Transiver::SetTxAddress(0x12345678, 0x90);

	Buttons::SetConfiguration(Buttons::PullUpOrDownIn);
	Buttons::Write(0x0);

    Pa4::Set();
   // debug << "Hello!!!\r\n";
	while(1)
	{
		keys = Buttons::PinRead();
		debug << keys << "\r\n";
		Transiver::DumpRegs(debug);
		Transiver::ClearInterruptStatus();
		Transiver::Write(&keys, 2);
		Util::delay_ms<10000, F_CPU>();
	}
}
