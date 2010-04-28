#include <avr/io.h>

#include "usart.h"
#include "util.h"
TextFormater< Usart<16, 32> > Debug;

#include "Crc16.h"
#include "MkiiProtocol.h"
#include "Pdi.h"
#include "ports.h"

#include "UsbDescriptors.h"
#include "UsbFifo.h"

using namespace MkII;
using namespace IO;



typedef UsbFifo interface;

typedef MkIIProtocol
	<
		interface, 
		PdiSoftwarePhisical<Pd5, Pd6>
		> protocol;


extern "C" uchar usbFunctionDescriptor(Usb::usbRequest_t *rq) 
{
	uint8_t len;
	//Debug << "D\n\r";
	switch(rq->wValue.bytes[1])
	{
		case USBDESCR_DEVICE:
			len = sizeof(Usb::DiviceDescriptor);
            Usb::usbMsgPtr = (uchar *)(&Usb::DiviceDescriptor);
			break;
		case USBDESCR_CONFIG:
			len = sizeof(Usb::Configuration);
            Usb::usbMsgPtr = (uchar *)(&Usb::Configuration);
			break;
		case USBDESCR_STRING:
		switch(rq->wValue.bytes[0])
		{
			case 0:
				len = sizeof(Usb::DescriptorString0);
				Usb::usbMsgPtr = (uchar *)(Usb::DescriptorString0);
			break;
			case 1:
				len = sizeof(Usb::Vendor);
				Usb::usbMsgPtr = (uchar *)(&Usb::Vendor);
			break;
			case 2:
				len = sizeof(Usb::Prodict);
				Usb::usbMsgPtr = (uchar *)(&Usb::Prodict);
			break;
			case 3:
				len = sizeof(Usb::SerialNumber);
				Usb::usbMsgPtr = (uchar *)(&Usb::SerialNumber);
			break;
		}
	}
	return len;
}

extern "C" usbMsgLen_t usbFunctionSetup(uint8_t data[8])
{
	Usb::usbRequest_t *rq = reinterpret_cast<Usb::usbRequest_t *>(data);
	return 0; 
}


ISR(USART_UDRE_vect)
{
	Debug.TxHandler();
}

ISR(USART_RXC_vect)
{
	Debug.RxHandler();
}


int main()
{
	Usb::usbInit();
	Debug.Init(115200);
	//Debug << "M\r\n";

    usbDeviceDisconnect();  /* enforce re-enumeration, do this while interrupts are disabled! */
    for(uint8_t i = 0; i<255; i++)
        _delay_ms(1);
    usbDeviceConnect();

	sei();

	IO::Portb::DirWrite(0xff);
	//protocol::Init();
	
	sei();
//	uint8_t buffer[] = "12345678";
	Usb::usbSetInterrupt(0, 0);
	while(1)
	{
		uint8_t c;
		
		//interface::Getch(c);
		//Debug.Putch(c);

		if(Debug.Getch(c))
		{
			Debug.Putch(c);
			while(!interface::Putch(c));
			
		}
		else
		{
		Debug.Putch(c);
			interface::EndTxFrame();
			
		}

		//if(Usb::InterruptIsReady())
		//	Usb::usbSetInterrupt(buffer, 7);
		
		//protocol::PollInterface();
		//interface::Putch('A');
		//interface::Putch('B');
		Usb::usbPoll();
	}
	return 0;
}

extern "C" void __cxa_pure_virtual() { while (1); }
