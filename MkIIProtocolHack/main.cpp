#include <avr/io.h>
#include "usart.h"
#include "util.h"
#include "ports.h"
#include "usbdrv.h"
#include <avr/interrupt.h>  /* for sei() */
#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include <util/delay.h>

typedef Usart<8, 4> usart;
typedef IO::Pd5 led;

// USB Configuration Descriptor 

PROGMEM char usbDescriptorConfiguration[] = 
{	
	9,		  				// length of descriptor in bytes
	USBDESCR_CONFIG,		// descriptor type 
	18 + 7 * 2 * USB_CFG_HAVE_INTRIN_ENDPOINT + 
			(USB_CFG_DESCR_PROPS_HID & 0xff), 0,
							// total length of data returned (including inlined descriptors)

	1,						// number of interfaces in this configuration
	1,						// index of this configuration
	0,						// configuration name string index 
#if USB_CFG_IS_SELF_POWERED
    (1 << 7) | USBATTR_SELFPOWER,
#else
    (1 << 7),
#endif
	USB_CFG_MAX_BUS_POWER/2,// max USB current in 2mA units 

//interface descriptor follows inline:
	9,		  				// sizeof(usbDescrInterface): length of descriptor in bytes
	USBDESCR_INTERFACE, 	// descriptor type
	0,		  				// index of this interface
	0,		  				// alternate setting for this interface
	2 * USB_CFG_HAVE_INTRIN_ENDPOINT,   // endpoints excl 0: number of endpoint descriptors to follow
	USB_CFG_INTERFACE_CLASS,
	USB_CFG_INTERFACE_SUBCLASS,
	USB_CFG_INTERFACE_PROTOCOL,
	0,		  				// string index for interface

	7,		  				// sizeof(usbDescrEndpoint)
	USBDESCR_ENDPOINT,  	// descriptor type = endpoint
	0x82,	   				// IN endpoint number 1
	0x02,	   				// attrib: Bulk endpoint
	8, 0,	   				// maximum packet size
	USB_CFG_INTR_POLL_INTERVAL, 

	7,
	USBDESCR_ENDPOINT,
	0x02,
	0x02,
	8, 0,
	USB_CFG_INTR_POLL_INTERVAL,
};


struct UsbDeviceDescriptor
{
	uint8_t bLength;			// 0x12 The size of this descriptor is 18 bytes
	uint8_t bDescriptorType;	// 0x01 DEVICE Descriptor Type
	uint16_t bcdUSB;			// 0x0110 Device compliant to the USB specification version 1.10
	uint8_t bDeviceClass;		// 0xFF The device class is vendor-specific
	uint8_t bDeviceSubClass;	// 0x00 Each interface specifies its own subclass information
	uint8_t bDeviceProtocol;	// 0x00 No protocols on the device basis
	uint8_t bMaxPacketSize0;	// 0x10 Maximum packet size for endpoint zero is 16
	uint16_t idVendor;			// 0x03EB Vendor ID is 1003: Atmel Corporation
	uint16_t idProduct;			// 0x2103 The Product ID is 0x2103
	uint16_t bcdDevice;			// 0x0200 The device release number is 2.00
	uint8_t iManufacturer;		// 0x01 The index of the string descriptor describing the manufacturer is 1
	uint8_t iProduct;			// 0x02 The index of the string descriptor describing the product is 2
	uint8_t iSerialNumber;		// 0x03 The index of the string descriptor describing the serial number is 3
	uint8_t bNumConfigurations;	// 0x01 The device has 1 possible configurations
};

struct UsbConfigurationDescriptor
{
	uint8_t bLength;			// 0x09 The size of this descriptor is 9 bytes
	uint8_t bDescriptorType;	// 0x02 CONFIGURATION Descriptor Type
	uint16_t wTotalLength;		// 0x0020 The total length of data for this configuration is 32. This includes the combined length of all the descriptors returned
	uint8_t bNumInterfaces;		// 0x01 This configuration supports 1 interfaces
	uint8_t bConfigurationValue;// 0x01 The value 1 should be used to select this configuration
	uint8_t iConfiguration;		// 0x00 The device doesn't have the string descriptor describing this configuration
	uint8_t bmAttributes;		// 0x80 Configuration characteristics:Bit 7: Reserved
	uint8_t MaxPower;			// 0xFA Maximum power consumption of the device in this configuration is 500 mA
};


struct UsbInterfaceDescriptor
{
	uint8_t bLength;			// 0x09 The size of this descriptor is 9 bytes
	uint8_t bDescriptorType;	// 0x04 INTERFACE Descriptor Type
	uint8_t bInterfaceNumber;	// 0x00 The number of this interface is 0
	uint8_t bAlternateSetting;	// 0x00 The value used to select alternate setting for this interface is 0
	uint8_t bNumEndpoints;		// 0x02 The number of endpoints used by this interface is 2(excluding endpoint zero)
	uint8_t bInterfaceClass;	// 0xFF The interface class is vendor-specific
	uint8_t bInterfaceSubClass;	// 0x00 The subclass code is 0x00
	uint8_t bInterfaceProtocol;	// 0x00 The interface doesn't use any class-specific protocols
	uint8_t iInterface;			// 0x00 The device doesn't have the string descriptor describing this interface
};

struct UsbEndpointDescriptor
{
	uint8_t bLength;			// 0x07 The size of this descriptor is 7 bytes
	uint8_t bDescriptorType;	// 0x05 ENDPOINT Descriptor Type
	uint8_t bEndpointAddress;	// 0x02 This is an OUT endpoint with address (endpoint number) 2
	uint8_t bmAttributes;		// 0x02 Types –Transfer:BULKSync:No SyncUsage:Data EP
	uint16_t wMaxPacketSize;	// 0x0040 Maximum packet size value for this endpoint is 0x40(Bits 12-11: Addtl. Transactions/frame)
	uint8_t bInterval;			// 0x0A bInterval:10. The polling interval value is bInterval or 2**(bInterval-1)
};


void usbFunctionWriteOut(uint8_t *data, uint8_t len)
{
	for(uint8_t i=0; i<min<uint8_t>(8, len); i++)
		usart::Putch(data[i]);
}



USB_PUBLIC usbMsgLen_t  usbFunctionSetup(uint8_t data[8])
{
	return 0; 
}

uchar usbFunctionDescriptor(usbRequest_t *rq) 
{
	usbMsgPtr = (uint8_t*)usbDescriptorConfiguration;
	return sizeof(usbDescriptorConfiguration);
}

// -------------------------------------------------------------------------

inline void BulkWrite()
{
	if(usbInterruptIsReady())// only if previous data was sent
	{               
    	uchar *p;
    	uchar len = getInterruptData(&p);   // obtain chunk of max 8 bytes
    	if(len > 0)                         // only send if we have data
       	usbSetInterrupt(p, len);
	}
}

int	main(void)
{
	uchar   i;
	led::SetDirWrite();
	usart::Init(115200);

	usbInit();
    usbDeviceDisconnect();  /* enforce re-enumeration, do this while interrupts are disabled! */
    i = 0;
    while(--i){             /* fake USB disconnect for > 250 ms */
        _delay_ms(1);
    }
    usbDeviceConnect();
	sei();
	for(;;) /* main event loop */
	{               
		
		BulkWrite();
		usbPoll();
	}
	return 0;
}


