#pragma once

namespace UsbInt
{
	extern "C"
	{
		#include "usbdrv.h"
	}
}


namespace Usb
{
	using namespace UsbInt;

	// values for UsbConfigurationDescriptor.bmAttributes
	enum ConfigAttributes
	{
		BusPowered = 0,
		SelfPowered = (1 << 6) | (1 << 7) ,
		RemoteWakeup = (1 << 5) | (1 << 7)
	};

	enum EndpointAttributes
	{
		Control 		= 0,
		Isochronous 	= (1 << 0),
		Bulk 			= (1 << 1),
		Interrupt		= (1 << 0) | (1 << 1),
	};

	enum DescriptorTypes
	{
		DeviceDescriptor = 0x01,
		ConfigurationDescriptor = 0x02,
		StringDescriptor = 0x03,
		InterfaceDescriptor = 0x04,
		EndpointDescriptor = 0x05
	};

	struct UsbDeviceDescriptor
	{
		uint8_t bLength;			// 0x12 The size of this descriptor is 18 bytes
		uint8_t bDescriptorType;	// 0x01 DEVICE Descriptor Type
		uint16_t bcdUSB;
		uint8_t bDeviceClass;
		uint8_t bDeviceSubClass;
		uint8_t bDeviceProtocol;
		uint8_t bMaxPacketSize0;
		uint16_t idVendor;
		uint16_t idProduct;
		uint16_t bcdDevice;
		uint8_t iManufacturer;
		uint8_t iProduct;
		uint8_t iSerialNumber;
		uint8_t bNumConfigurations;
	};

	struct UsbConfigurationDescriptor
	{
		uint8_t bLength;			// 0x09 The size of this descriptor is 9 bytes
		uint8_t bDescriptorType;	// 0x02 CONFIGURATION Descriptor Type
		uint16_t wTotalLength;		// The total length of data for this configuration is 32. This includes the combined length of all the descriptors returned
		uint8_t bNumInterfaces;		
		uint8_t bConfigurationValue;
		uint8_t iConfiguration;		// 0x00 The device doesn't have the string descriptor describing this configuration
		uint8_t bmAttributes;		// Configuration characteristics:Bit 7: Reserved
		uint8_t MaxPower;			// Maximum power consumption of the device in this configuration is 500 mA
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
		uint16_t wMaxPacketSize;	//  Maximum packet size value for this endpoint is 0x40(Bits 12-11: Addtl. Transactions/frame)
		uint8_t bInterval;			// 0x0A bInterval:10. The polling interval value is bInterval or 2**(bInterval-1)
	};

	template<class DataSource>
	void usbSetInterruptDataGeneric(uchar len, usbTxStatus_t *txStatus)
	{
	uchar   *p, c;
	char    i;

	#if USB_CFG_IMPLEMENT_HALT
	    if(usbTxLen1 == USBPID_STALL)
	        return;
	#endif
	    if(txStatus->len & 0x10)// packet buffer was empty
		{    
	        txStatus->buffer[0] ^= USBPID_DATA0 ^ USBPID_DATA1; // toggle token 
	    }
		else
		{
	        txStatus->len = USBPID_NAK; // avoid sending outdated (overwritten) interrupt data 
	    }
	    p = txStatus->buffer + 1;
	    i = len;
	    do 	//if len == 0, we still copy 1 byte, but that's no problem
		{      
			DataSource::Getch(c);
	        *p++ = c;
	    }while(--i > 0);
	    usbCrc16Append(&txStatus->buffer[1], len);
	    txStatus->len = len + 4;    // len must be given including sync byte
	}

	template<class DataSource>
	void usbSetInterrupt(uchar len)
	{
	    usbSetInterruptDataGeneric<DataSource>(len, &usbTxStatus1);
	}

}

#define STRING_DESCRIPTOR(name, value) \
	struct \
	{\
		uint8_t bLength;\
		uint8_t bDescriptorType;\
		wchar_t bString[sizeof(value)/2];\
	} name = \
		{\
			sizeof(value) + 2,\
			Usb::StringDescriptor,\
			value\
		}\


