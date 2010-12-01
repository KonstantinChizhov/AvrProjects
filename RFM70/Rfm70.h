#pragma once 

#include <iopins.h>
#include <static_assert.h>
#include <util/delay.h>

static const bool IsLSBitFirst = true;
static const bool IsLSByteFirst = true;

enum Command
{
	ReadRegCmd        	= 0x00,
	WriteRegCmd       	= 0x20,
	ReadRxDataCmd	   	= 0x61,
	WriteTxDataCmd     	= 0xA0,
	FlushTxCmd	       	= 0xE1,
	FlushRxCmd	       	= 0xE2,
	ReuseTxDataCmd     	= 0xE3,
	WriteTxDataNoAckCmd	= 0xb0,
	WriteAckDataCmd		= 0xa8,
	ActivateCmd			= 0x50,
	ReadRxDataLenghtCmd	= 0x60,
	NopCmd            	= 0xFF 
};

enum Registers
{
	ConfigReg			= 0x00,
	EnableAutoAckReg	= 0x01,
	EnableRxAdressReg	= 0x02,
	SetupAdressWidthReg	= 0x03,
	SetupRetryReg		= 0x04,
	RfChannelReg		= 0x05,
	RfSetupReg			= 0x06,
	StatusReg			= 0x07,
	ObserveTxReg		= 0x08,
	CarrierDetectReg	= 0x09,
	RxAddress0			= 0x0A,
	RxAddress1			= 0x0B,
	RxAddress2			= 0x0C,
	RxAddress3			= 0x0D,
	RxAddress4			= 0x0E,
	RxAddress5			= 0x0F,
	TxAddress			= 0x10,
	RxDataLength0		= 0x11,
	RxDataLength1		= 0x12,
	RxDataLength2		= 0x13,
	RxDataLength3		= 0x14,
	RxDataLength4		= 0x15,
	RxDataLength5		= 0x16,
	FifoStatusReg  		= 0x17
};


enum FifoStatus
{
	FifoTxReuse		= 0x40,
	FifoTxFull		= 0x20,
	FifoTxEmpty		= 0x10,
	FifoRxFull		= 0x02,
	FifoRxEmpty		= 0x01
};

enum ConfigRegValues
{
	MaskRxDataReady	= 1 << 6,
	MaskTxDataReady	= 1 << 5,
	MaskMaxRetry	= 1 << 4,
	EnableCrc		= 1 << 3,
	Crc2bytes		= 1 << 2,
	PowerUpBit		= 1 << 1,
	TxModeBit		= 1 << 0 
};

enum{Reg15Size = 11 };
const uint8_t Bank1_Reg15[Reg15Size]=
{
	0x41,0x20,0x08,0x04,0x81,0x20,0xCF,0xF7,0xFE,0xFF,0xFF //LSB first
};

enum AddressWidthValues
{
	AW3Bytes = 1,
	AW4Bytes = 2,
	AW5Bytes = 3
};

enum AutoRetransmissionDelayValues
{
	 Wait250us = 0x0,
	 Wait500us = 0x1,
	 Wait750us = 0x2,
	 Wait1000us = 0x3,
	 Wait1250us = 0x4,
	 Wait1500us = 0x5,
	 Wait1750us = 0x6,
	 Wait2000us = 0x7,
	 Wait2250us = 0x8,
	 Wait2500us = 0x9,
	 Wait2750us = 0xA,
	 Wait3000us = 0xB,
	 Wait3250us = 0xC,
	 Wait3500us = 0xD,
	 Wait3750us = 0xE,
	 Wait4000us = 0xF
};

enum RfSetupValues
{
	DataRate1Mbps 		= 0 << 3,
	DataRate2Mbps 		= 1 << 3,

	OutputPowerMinus10dBm = 0 << 1,
	OutputPowerMinus5dBm = 1 << 1,
	OutputPower0dBm 	= 2 << 1,
	OutputPower5dBm 	= 3 << 1,

	LnaLowGain			= 0,
	LnaHighGain			= 1
};

enum StatusValues
{
	RegBank			= 1 << 7,
	RxDataReady		= 1 << 6,
	TxDataSent		= 1 << 5,
	MaxRetransmits	= 1 << 4,
	RxPipeNumberMask= 7 << 1,
	RxPipeNumberShift= 1,
	TxFull			= 1
};

class Rfm70Defaults
{
	static const AutoRetransmissionDelayValues AutoRetransmissionDelay = Wait2000us;
	static const uint8_t AutoRetransmissionCount = 15; //0..15
public:
	static const AddressWidthValues AddressWidth = AW5Bytes;

	static const uint8_t RfChannel	= 12; // 0..83
	static const uint8_t RfSetup 	= DataRate2Mbps | OutputPower5dBm | LnaHighGain;
	static const uint8_t Config 	= EnableCrc | Crc2bytes;

public:
	static const uint8_t RetrySetrup = Wait2000us | (AutoRetransmissionCount << 4);

};


template<class Spi, class SlaveSelectPin, class EnablePin, class IrqPin, class Defaults = Rfm70Defaults>
class Rfm70
{
private:

	static void WriteReg(uint8_t reg, uint8_t value)
	{
		SlaveSelectPin::Clear();
		Spi::ReadWrite(reg);
		Spi::ReadWrite(value);
		SlaveSelectPin::Set();
	}

	static uint8_t ReadReg(uint8_t reg)                            
	{                                                           
		SlaveSelectPin::Clear();
		Spi::ReadWrite(reg);
		uint8_t value = Spi::ReadWrite(0);
		SlaveSelectPin::Set();
		return value;
	}

	static void WriteReg32(uint8_t reg, uint32_t value)
	{
		union
		{
			uint32_t dword;
			uint8_t byte[4];
		};
		dword = value;

		SlaveSelectPin::Clear();
		Spi::ReadWrite(reg);

		Spi::ReadWrite(byte[0]);
		Spi::ReadWrite(byte[1]);
		Spi::ReadWrite(byte[2]);
		Spi::ReadWrite(byte[3]);
		
		SlaveSelectPin::Set();
	}

	static uint32_t ReadReg32(uint8_t reg)
	{                                                           
		SlaveSelectPin::Clear();
		Spi::ReadWrite(reg);
		uint32_t value = Spi::ReadWrite(0);
		value |= Spi::ReadWrite(0) << 8;
		value |= Spi::ReadWrite(0) << 16;
		value |= Spi::ReadWrite(0) << 24;
		SlaveSelectPin::Set();
		return value;
	}

	static void ReadBuffer(uint8_t command, uint8_t *buffer, uint8_t length)
	{
		SlaveSelectPin::Clear();
		Spi::ReadWrite(command);

		for(uint8_t *end = buffer + length; buffer != end; ++buffer)
			*buffer = Spi::ReadWrite(0);

		SlaveSelectPin::Set();
	}

	static void WriteBuffer(uint8_t command, const uint8_t *buffer, uint8_t length)
	{
		SlaveSelectPin::Clear();
		Spi::ReadWrite(command);

		for(const uint8_t *end = buffer + length; buffer != end; ++buffer)
			Spi::ReadWrite(*buffer);

		SlaveSelectPin::Set();
	}

	static void InitBank1Regs()
	{
		SwitchBank(1);
		WriteBuffer(WriteRegCmd | 15, Bank1_Reg15, Reg15Size);

		WriteReg32(WriteRegCmd | 0x00, 0xE2014B40);
		WriteReg32(WriteRegCmd | 0x01, 0x00004BC0);
		WriteReg32(WriteRegCmd | 0x02, 0x028CFCD0);
		WriteReg32(WriteRegCmd | 0x03, 0x41390099);
		WriteReg32(WriteRegCmd | 0x04, 0x0B869ED9);
		WriteReg32(WriteRegCmd | 0x05, 0xA67F0624);
		WriteReg32(WriteRegCmd | 0x06, 0x0B869ED9);

		WriteReg32(WriteRegCmd | 0x0c, 0x00731200);
		WriteReg32(WriteRegCmd | 0x0d, 0x0080B436);
	}

	static void SwitchBank(bool bank)
	{
		bool tmp = (ReadReg(ReadRegCmd | 7) & 0x80) != 0;
		if(tmp != bank)		
		{
			WriteReg(ActivateCmd, 0x53);
		}
	}

public:

	static void Init()
	{
		SlaveSelectPin::Set();
		EnablePin::Set();
		SlaveSelectPin::SetDirWrite();
		EnablePin::SetDirWrite();
		_delay_ms(50);

		Activate();

	//	InitBank1Regs();
		
		SwitchBank(0);

		WriteReg(WriteRegCmd | ConfigReg, Defaults::Config | PowerUpBit);

		WriteReg(WriteRegCmd | SetupAdressWidthReg, Defaults::AddressWidth);
		RfChannel(Defaults::RfChannel);
		RfSetup(Defaults::RfSetup);
		WriteReg(WriteRegCmd | SetupRetryReg, Defaults::RetrySetrup);
		WriteReg(WriteRegCmd | RxDataLength0, 32);
		
	}

	static void PowerUp()
	{
		uint8_t config = ReadReg(ReadRegCmd | ConfigReg);
	  	WriteReg(WriteRegCmd | ConfigReg, config | PowerUpBit); 
	}

	static void PowerDown()
	{
		uint8_t config = ReadReg(ReadRegCmd | ConfigReg);
	  	WriteReg(WriteRegCmd | ConfigReg, config & ~PowerUpBit); 
	}

	static void SwitchToRxMode()
	{
		FlushRx();
		uint8_t value = ReadReg(ReadRegCmd | StatusReg);
		WriteReg(WriteRegCmd | StatusReg, value);

		EnablePin::Clear();
		value = ReadReg(ReadRegCmd | ConfigReg);
	  	WriteReg(WriteRegCmd | ConfigReg, value | TxModeBit); 
		EnablePin::Set();
	}

	static void SwitchToTxMode()
	{
		FlushTx();
		EnablePin::Clear();
		uint8_t value = ReadReg(ConfigReg);
	  	WriteReg(WriteRegCmd | ConfigReg, value & ~TxModeBit);
		EnablePin::Set();
	}

	static void RfChannel(uint8_t channel)
	{
		WriteReg(WriteRegCmd | RfChannelReg, channel);
	}

	static void RfSetup(uint8_t rfSetup)
	{
		WriteReg(WriteRegCmd | RfSetupReg, rfSetup);
	}
	
	static void FlushTx()
	{
		Spi::ReadWrite(FlushTxCmd);
	}
	
	static void FlushRx()
	{
		Spi::ReadWrite(FlushRxCmd);
	}
	
	static void Activate()
	{
		WriteReg(ActivateCmd, 0x73);
	}
	
	static uint8_t RecivedDataLength()
	{
		return ReadReg(ReadRxDataLenghtCmd);
	}
	
	static uint8_t DataLength(uint8_t pipe)
	{
		return ReadReg(ReadRegCmd | (RxDataLength0 + pipe & 0x1f));
	}
	
	static uint8_t ActiveRxPipe()
	{
		return (ReadReg(ReadRegCmd | StatusReg) >> RxPipeNumberShift) & RxPipeNumberMask;
	}

	static uint8_t Status()
	{
		return ReadReg(ReadRegCmd | StatusReg);
	}

	static void ClearInterruptStatus()
	{
		WriteReg(WriteRegCmd | StatusReg, Status());
	}
	
	static bool Write(const void * buffer, uint8_t size)
	{
		SwitchToTxMode(); 

		uint8_t fifoStatus =ReadReg(ReadRegCmd | FifoStatusReg);
		if(!(fifoStatus & FifoTxFull))
		{	
			MyFormater debug;
			debug << "Write " << size << " bytes\r\n";
			WriteBuffer(WriteTxDataCmd, (uint8_t*)buffer, size);
		}
	}
	
	/// Reads recived data payload.
	static uint8_t Recive(void * buffer)
	{
		uint8_t length = RecivedDataLength();
		uint8_t pipe = ActiveRxPipe();
		MyFormater debug;
		
		if(Status() & RxDataReady)
		{
			debug << "Recived " << length << " bytes at pipe" << pipe << "\r\n";
			ReadBuffer(ReadRxDataCmd, (uint8_t*)buffer, length);
			WriteReg(WriteRegCmd | StatusReg, Status());
		}
		else
		{
			return 0xff;
		}
		return pipe;
	}
		
	template<class Formater>
	static void DumpRegs()
	{
		Formater formater;
		SwitchBank(0);
		formater << "Bank0:\r\n";

		uint8_t buffer[12];
	
		for(unsigned i = 0; i<32; i++)
		{
			if(i==0x0A || i==0x0B || i==0x10)
			{
				ReadBuffer(ReadRegCmd | i, buffer, 5);
				formater << i << ":\t0x";
				for(unsigned i = 5; i; i--)
					formater  << buffer[i-1];
				formater << "\r\n";
			}
			else
			{
				unsigned val = ReadReg(ReadRegCmd | i);
				formater << i << ":\t" << val << "\r\n";
			}
		}
	}

	template<class Formater>
	static void DumpRegs1()
	{
		Formater formater;
		SwitchBank(1);
		formater << "Bank1:\r\n";

		for(unsigned i = 0; i<14; i++)
		{
			unsigned long val;
			ReadBuffer(ReadRegCmd | i, (uint8_t*)(&val), 4);
			formater << "0x" << i << ":\t0x" << val << "\r\n";
		}
		uint8_t buffer[12];
		ReadBuffer(ReadRegCmd | 14, buffer, 11);
		formater << "0xe:\t0x";
		for(unsigned i = 11; i; i--)
			formater  << buffer[i-1];
		formater << "\r\n";
		SwitchBank(0);
	}
};

