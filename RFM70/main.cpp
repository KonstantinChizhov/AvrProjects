#include <avr/io.h>
#include "Spi.h"

#include <TextFormater.h>
#include <usart.h>
#include <util/delay.h>

typedef WaitAdapter<Usart<16, 16> >  MyUsart;



typedef TextFormater<MyUsart, 16> MyFormater;

#include "Rfm70.h"

typedef Spi<> MySpi;

typedef SoftSpi<IO::Pc2, IO::Pc1, IO::Pc3> Spi2;

typedef Rfm70<MySpi, IO::Pb0, IO::Pb1, IO::Pb2> Transiver1;
typedef Rfm70<Spi2, IO::Pc4, IO::Pc5, IO::Pc0> Transiver2;

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
	uint8_t buffer1[32] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	uint8_t buffer2[32];

	Debug::SetDirWrite();

	MyUsart::Init(19200);
	MySpi::Init(MySpi::Div16);
	//sei();
	MyFormater formater;
	formater.PutsP(PSTR("Rfm70 test programm\r\n"));

	Transiver1::Init();
	Transiver2::Init();
	
	Transiver1::SwitchToRxMode();

	Transiver2::SwitchToTxMode();
	
formater << "Rx\t";
	Transiver1::DumpRegs<MyFormater>();
	//Transiver1::DumpRegs1<MyFormater>();
formater << "Tx\t";
	Transiver2::DumpRegs<MyFormater>();
	//Transiver2::DumpRegs1<MyFormater>();

	Transiver2::Write(buffer1, 32);
	formater << "Rx\t";
	Transiver1::DumpRegs<MyFormater>();
	formater << "Tx\t";
	Transiver2::DumpRegs<MyFormater>();

	_delay_ms(100);
	Transiver1::Recive(buffer2);

	formater << "Rx\t";
	Transiver1::DumpRegs<MyFormater>();
	formater << "Tx\t";
	Transiver2::DumpRegs<MyFormater>();

	_delay_ms(100);
	Transiver1::Recive(buffer2);
	_delay_ms(100);
	Transiver1::Recive(buffer2);
	
	formater << "Rx\t";
	Transiver1::DumpRegs<MyFormater>();
	formater << "Tx\t";
	Transiver2::DumpRegs<MyFormater>();

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
