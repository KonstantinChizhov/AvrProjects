#include <avr/io.h>
#include "Spi.h"
#include "Rfm70.h"
#include <TextFormater.h>
#include <usart.h>
#include <util/delay.h>

typedef WaitAdapter<Usart<16, 16> >  MyUsart;
typedef TextFormater<MyUsart, 16> MyFormater;
typedef Spi<> MySpi;

typedef SoftSpi<IO::Pb5, IO::Pb6, IO::Pb7> Spi2;

typedef Rfm70<Spi2, IO::Pb0, IO::Pb1, IO::Pb2> Transiver1;

typedef IO::Pa0 Debug;

ISR(USART_RXC_vect)
{
	MyUsart::RxHandler();
}

ISR(USART_UDRE_vect)
{
	//Debug::Set();
	MyUsart::TxHandler();
	//Debug::Clear();
}
__attribute__((OS_main))

int main()
{
	Debug::SetDirWrite();

	MyUsart::Init(19200);
	MySpi::Init(MySpi::Div16);
	//sei();
	MyFormater formater;
	formater.PutsP(PSTR("Rfm70 test programm\r\n"));

	Transiver1::Init();

	Transiver1::DumpRegs<MyFormater>();
	Transiver1::DumpRegs1<MyFormater>();
	//Transiver1::WriteReg(WRITE_REG | CONFIG, 100);
	//Transiver1::DumpRegs<MyFormater>();
	
	while(1)
	{
		//Transiver1::RfChannel(10);
		//MyUsart::Write(MyUsart::Read());
		//Transiver1::WriteReg(WRITE_REG | 0, v++);
		//_delay_us(200);
		//MyUsart::Write(Transiver1::ReadReg(READ_REG | 0));

	//	_delay_ms(200);
	}
}
