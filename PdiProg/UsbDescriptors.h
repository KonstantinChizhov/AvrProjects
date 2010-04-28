#pragma once

#include "usbdrvCpp.h"

namespace Usb
{

	PROGMEM Usb::DeviceDescriptorT DiviceDescriptor =
	{
		sizeof(Usb::DeviceDescriptorT),	
		Usb::DeviceDescriptor,	
		0x0110,	// Device compliant to the USB specification version 1.10
		0xFF,	// The device class is vendor-specific
		0x00,	// Each interface specifies its own subclass information
		0x00,	// No protocols on the device basis
		8,		// Maximum packet size for endpoint zero
		0x16c0,//0x03EB,	// Vendor ID is 1003: Atmel Corporation
		0x5dc,//0x2103,	// The Product ID is 0x2103
		0x0200,	// The device release number is 2.00
		0x01,	// The index of the string descriptor describing the manufacturer is 1
		0x02,	// The index of the string descriptor describing the product is 2
		0x03,	// The index of the string descriptor describing the serial number is 3
		0x01	// Number of configurations
	};

	struct ConfigurationT
	{
		Usb::ConfigurationDescriptorT congig;
		Usb::InterfaceDescriptorT interface;
		Usb::EndpointDescriptorT endpoint1;
		Usb::EndpointDescriptorT endpoint2;
	};

	PROGMEM ConfigurationT Configuration =
	{
		{
			sizeof(Usb::ConfigurationDescriptorT),// 0x09 The size of this descriptor is 9 bytes
			Usb::ConfigurationDescriptor,		// 0x02 CONFIGURATION Descriptor Type
			sizeof(ConfigurationT),			// The total length of data for this configuration
			1,
			1,
			0,									// 0x00 The device doesn't have the string descriptor describing this configuration
			Usb::BusPowered,					// Configuration characteristics:Bit 7: Reserved
			500 / 2 							// Maximum power consumption of the device
		},
		{
			sizeof(Usb::InterfaceDescriptorT),// 0x09 The size of this descriptor is 9 bytes
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
			sizeof(Usb::EndpointDescriptorT),	// The size of this descriptor is 7 bytes
			Usb::EndpointDescriptor,			// ENDPOINT Descriptor Type
			0x82,								// This is an OUT endpoint with address (endpoint number) 2
			Usb::Bulk,							// Types –Transfer:BULKSync:No SyncUsage:Data EP
			8,									// Maximum packet size value for this endpoint is 0x40(Bits 12-11: Addtl. Transactions/frame)
			0									// The polling interval value is bInterval or 2**(bInterval-1)
		},
		{
			sizeof(Usb::EndpointDescriptorT),	// The size of this descriptor is 7 bytes
			Usb::EndpointDescriptor,			// ENDPOINT Descriptor Type
			0x02,								// This is an IN endpoint with address (endpoint number) 2
			Usb::Bulk,							// Types –Transfer:BULKSync:No SyncUsage:Data EP
			8,									// Maximum packet size value for this endpoint is 0x40(Bits 12-11: Addtl. Transactions/frame)
			0									// The polling interval value is bInterval or 2**(bInterval-1)
		}
	};


	PROGMEM char DescriptorString0[] = {
	    4,          /* sizeof(usbDescriptorString0): length of descriptor in bytes */
	    3,          /* descriptor type */
	    0x09, 0x04, /* language index (0x0409 = US-English) */
	};

	PROGMEM STRING_DESCRIPTOR(Vendor, "Atmel");
	PROGMEM STRING_DESCRIPTOR(Prodict, "JTAGICE mkII");
	PROGMEM STRING_DESCRIPTOR(SerialNumber, "00A20000011E0");

}
