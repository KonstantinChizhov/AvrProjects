#include <avr/io.h>
#include <spi.h>

#include <TextFormater.h>
#include <usart.h>
#include <util/delay.h>

typedef WaitAdapter<Usart<16, 16> >  MyUsart;


typedef TextFormater<MyUsart, 16> MyFormater;

#include <Rfm70.h>


typedef SoftSpi<IO::Pc2, IO::Pc1, IO::Pc3> Spi2;

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
	char buffer[32];

	Debug::SetDirWrite();

	MyUsart::Init(19200);

	sei();
	MyFormater formater;
	formater.PutsP(PSTR("Rfm70 test programm\r\n"));

	Transiver2::Init();
	
	Transiver2::SwitchToRxMode();
	
	//formater << "Rx\t";
	//Transiver2::DumpRegs<MyFormater>();


	while(1)
	{
		if(Transiver2::Recive(buffer) != 0xff)
			formater << buffer;
		_delay_ms(100);
	}
}
