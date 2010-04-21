#include <avr/io.h>
#include "usart.h"
#include "util.h"
#include "ports.h"
#include "usbdrvCpp.h"
#include <avr/interrupt.h>  /* for sei() */
#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include <util/delay.h>
#include <avr/wdt.h>

TextFormater< WaitAdapter< Usart<8, 4> > > usart;
typedef IO::Pd5 led;

// USB Configuration Descriptor 

PROGMEM Usb::UsbDeviceDescriptor diviceDescr =
{
	sizeof(Usb::UsbDeviceDescriptor),	
	Usb::DeviceDescriptor,	
	0x0110,	// Device compliant to the USB specification version 1.10
	0xFF,	// The device class is vendor-specific
	0x00,	// Each interface specifies its own subclass information
	0x00,	// No protocols on the device basis
	8,		// Maximum packet size for endpoint zero is 16
	0x03EB,	// Vendor ID is 1003: Atmel Corporation
	0x2103,	// The Product ID is 0x2103
	0x0200,	// The device release number is 2.00
	0x01,	// The index of the string descriptor describing the manufacturer is 1
	0x02,	// The index of the string descriptor describing the product is 2
	0x03,	// The index of the string descriptor describing the serial number is 3
	0x01	// Number of configurations
};

struct MyConfiguration
{
	Usb::UsbConfigurationDescriptor congig;
	Usb::UsbInterfaceDescriptor interface;
	Usb::UsbEndpointDescriptor endpoint1;
	Usb::UsbEndpointDescriptor endpoint2;
};

PROGMEM MyConfiguration myConfig =
{
	{
		sizeof(Usb::UsbConfigurationDescriptor),// 0x09 The size of this descriptor is 9 bytes
		Usb::ConfigurationDescriptor,		// 0x02 CONFIGURATION Descriptor Type
		sizeof(MyConfiguration),			// The total length of data for this configuration
		1,
		1,
		0,									// 0x00 The device doesn't have the string descriptor describing this configuration
		Usb::BusPowered,					// Configuration characteristics:Bit 7: Reserved
		500 / 2 							// Maximum power consumption of the device
	},
	{
		sizeof(Usb::UsbInterfaceDescriptor),// 0x09 The size of this descriptor is 9 bytes
		Usb::InterfaceDescriptor,			// 0x04 INTERFACE Descriptor Type
		0, 									// The number of this interface is 0
		0,									// The value used to select alternate setting for this interface is 0
		2,									// The number of endpoints used by this interface is 2(excluding endpoint zero)
		0xFF,								// The interface class is vendor-specific
		0x00,								// The subclass code is 0x00
		0x00,								// 0x00 The interface doesn't use any class-specific protocols
		0x00								// The device doesn't have the string descriptor describing this interface
	},
	{
		sizeof(Usb::UsbEndpointDescriptor),	// The size of this descriptor is 7 bytes
		Usb::EndpointDescriptor,			// ENDPOINT Descriptor Type
		0x82,								// This is an OUT endpoint with address (endpoint number) 2
		Usb::Bulk,							// Types –Transfer:BULKSync:No SyncUsage:Data EP
		8,									// Maximum packet size value for this endpoint is 0x40(Bits 12-11: Addtl. Transactions/frame)
		10									// The polling interval value is bInterval or 2**(bInterval-1)
	},
	{
		sizeof(Usb::UsbEndpointDescriptor),	// The size of this descriptor is 7 bytes
		Usb::EndpointDescriptor,			// ENDPOINT Descriptor Type
		0x02,								// This is an IN endpoint with address (endpoint number) 2
		Usb::Bulk,							// Types –Transfer:BULKSync:No SyncUsage:Data EP
		8,									// Maximum packet size value for this endpoint is 0x40(Bits 12-11: Addtl. Transactions/frame)
		10									// The polling interval value is bInterval or 2**(bInterval-1)
	}
};


PROGMEM char usbDescriptorString0[] = {
    4,          /* sizeof(usbDescriptorString0): length of descriptor in bytes */
    3,          /* descriptor type */
    0x09, 0x04, /* language index (0x0409 = US-English) */
};

PROGMEM STRING_DESCRIPTOR(Vendor, L"Atmel");
PROGMEM STRING_DESCRIPTOR(Prodict, L"JTAGICE mkII");
PROGMEM STRING_DESCRIPTOR(SerialNumber, L"00A20000011E0");


extern "C" uchar usbFunctionDescriptor(Usb::usbRequest_t *rq) 
{
usart.Putch('D');
	uint8_t len;
	switch(rq->wValue.bytes[1])
	{
		case USBDESCR_DEVICE:
			len = sizeof(diviceDescr);
            Usb::usbMsgPtr = (uchar *)(&diviceDescr);
			break;
		case USBDESCR_CONFIG:
			len = sizeof(myConfig);
            Usb::usbMsgPtr = (uchar *)(&myConfig);
			break;
		case USBDESCR_STRING:
		switch(rq->wValue.bytes[0])
		{
			case 0:
				len = sizeof(usbDescriptorString0);
				Usb::usbMsgPtr = (uchar *)(usbDescriptorString0);
			break;
			case 1:
				len = sizeof(Vendor);
				Usb::usbMsgPtr = (uchar *)(&Vendor);
			break;
			case 2:
				len = sizeof(Prodict);
				Usb::usbMsgPtr = (uchar *)(&Prodict);
			break;
			case 3:
				len = sizeof(SerialNumber);
				Usb::usbMsgPtr = (uchar *)(&SerialNumber);
			break;
		}
	}
	return len;
}

extern "C" void usbFunctionWriteOut(uint8_t *data, uint8_t len)
{
	//usart.Putch('W');
	for(uint8_t i=0; i<min<uint8_t>(8, len); i++)
		usart.Putch(data[i]);
}

extern "C" usbMsgLen_t usbFunctionSetup(uint8_t data[8])
{
	led::Togle();
	usart.Putch('F');
	return 0; 
}

// -------------------------------------------------------------------------

/*
inline void BulkWrite()
{
	if(usbInterruptIsReady())// only if previous data was sent
	{               
    	usbSetInterrupt<typeof(usart)>(usart.BytesRecived());
	}
}

*/

ISR(USART_RX_vect)
{
	usart.RxHandler();
}

ISR(USART_UDRE_vect)
{
	usart.TxHandler();
}


int	main(void)
{
	uint8_t i;
	led::SetDirWrite();
	usart.Init(115200);
	usart.Putch('M');
	Usb::usbInit();
    usbDeviceDisconnect();  /* enforce re-enumeration, do this while interrupts are disabled! */
    i = 0;
    while(--i){             /* fake USB disconnect for > 250 ms */
        _delay_ms(1);
    }
    usbDeviceConnect();
	sei();
	//usart << "Hello!\r\n";
	for(;;) /* main event loop */
	{               
		wdt_reset();
		//BulkWrite();
		Usb::usbPoll();
	}
	return 0;
}


