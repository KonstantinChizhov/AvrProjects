#pragma once
#include "usbdrvCpp.h"

class UsbFifo
{
	enum {UsbPacketSize = 8};
	static uint8_t FlushTx()
	{
		if(Usb::InterruptIsReady())
		{
			Usb::usbSetInterrupt(_txBuf, _txCount);
			_txCount = 0;
			return 1;
		}
		else return 0;
	}
public:

	static uint8_t Putch(uint8_t c)
	{
		if(_txCount < UsbPacketSize)
		{
			_txBuf[_txCount++] = c;
			if(_txCount == UsbPacketSize)
			{
				return FlushTx();
			}
		}else
		{
			if(!FlushTx())
				return 0;
			_txBuf[_txCount++] = c;
		}
	}

	static void Getch(uint8_t &c)
	{
		while(!_rxBuf.Read(c))
		{
			Usb::usbPoll();
		}
//		Debug.Putch('R');
//		Debug.Putch(c);
//		Debug.Putch(' ');
	}

	static void BeginTxFrame()
	{
		//Usb::usbSetInterrupt(0, 0);
	}

	static void EndTxFrame()
	{
		do
		{
			Usb::usbPoll();
		}while(!FlushTx());
	}

	static void BeginRx()
	{
		
	}

	static void EndRx()
	{
	
	}

	static void RxCallBack(uint8_t *data, uint8_t len)
	{
		for(uint8_t i=0; i < len; i++)
		{
			_rxBuf.Write(data[i]);
//			Debug.Putch(data[i]);
		}
	}

protected:
	static Queue<16> _rxBuf;
	static Array<8> _txBuf;
	static uint8_t _txCount;
};

	Queue<16> UsbFifo::_rxBuf;
	Array<8> UsbFifo::_txBuf;
	uint8_t UsbFifo::_txCount = 0;


extern "C" void usbFunctionWriteOut(uint8_t *data, uint8_t len)
{
//	Debug << len << "\r\n";
	UsbFifo::RxCallBack(data, len);
}
