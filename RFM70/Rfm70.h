#pragma once 

#include <iopins.h>
#include <static_assert.h>

enum Command
{
	READ_REG        	= 0x00,  // Define read command to register
	WRITE_REG       	= 0x20,  // Define write command to register
	RD_RX_PLOAD     	= 0x61,  // Define RX payload register address
	WR_TX_PLOAD     	= 0xA0,  // Define TX payload register address
	FLUSH_TX        	= 0xE1,  // Define flush TX register command
	FLUSH_RX        	= 0xE2,  // Define flush RX register command
	REUSE_TX_PL     	= 0xE3,  // Define reuse TX payload register command
	W_TX_PAYLOAD_NOACK	= 0xb0,
	W_ACK_PAYLOAD		= 0xa8,
	ACTIVATE			= 0x50,
	R_RX_PL_WID			= 0x60,
	NOP_NOP            	= 0xFF 
};

enum Registers
{
	CONFIG          = 0x00,  // 'Config' register address
	EN_AA           = 0x01,  // 'Enable Auto Acknowledgment' register address
	EN_RXADDR       = 0x02,  // 'Enabled RX addresses' register address
	SETUP_AW        = 0x03,  // 'Setup address width' register address
	SETUP_RETR      = 0x04,  // 'Setup Auto. Retrans' register address
	RF_CH           = 0x05,  // 'RF channel' register address
	RF_SETUP        = 0x06,  // 'RF setup' register address
	STATUS          = 0x07,  // 'Status' register address
	OBSERVE_TX      = 0x08,  // 'Observe TX' register address
	CD              = 0x09,  // 'Carrier Detect' register address
	RX_ADDR_P0      = 0x0A,  // 'RX address pipe0' register address
	RX_ADDR_P1      = 0x0B,  // 'RX address pipe1' register address
	RX_ADDR_P2      = 0x0C,  // 'RX address pipe2' register address
	RX_ADDR_P3      = 0x0D,  // 'RX address pipe3' register address
	RX_ADDR_P4      = 0x0E,  // 'RX address pipe4' register address
	RX_ADDR_P5      = 0x0F,  // 'RX address pipe5' register address
	TX_ADDR         = 0x10,  // 'TX address' register address
	RX_PW_P0        = 0x11,  // 'RX payload width, pipe0' register address
	RX_PW_P1        = 0x12,  // 'RX payload width, pipe1' register address
	RX_PW_P2        = 0x13,  // 'RX payload width, pipe2' register address
	RX_PW_P3        = 0x14,  // 'RX payload width, pipe3' register address
	RX_PW_P4        = 0x15,  // 'RX payload width, pipe4' register address
	RX_PW_P5        = 0x16,  // 'RX payload width, pipe5' register address
	FIFO_STATUS     = 0x17,  // 'FIFO Status Register' register address
	PAYLOAD_WIDTH   = 0x1f   // 'payload length of 256 bytes modes register address
};

enum IntStatus
{
	RX_DR 	= 0x40,
	TX_DS 	= 0x20,
	MAX_RT 	= 0x10
};

enum{ TX_FULL 	= 0x01};

enum FifoStatus
{
	FIFO_STATUS_TX_REUSE 	= 0x40,
	FIFO_STATUS_TX_FULL 	= 0x20,
	FIFO_STATUS_TX_EMPTY 	= 0x10,
	FIFO_STATUS_RX_FULL 	= 0x02,
	FIFO_STATUS_RX_EMPTY 	= 0x01
};
enum ConfigReg
{
	MASK_RX_DR	= 1 << 6,
	MASK_TX_DS	= 1 << 5,
	MASK_MAX_RT	= 1 << 4,
	EN_CRC		= 1 << 3,
	CRCO		= 1 << 2,
	PWR_UP		= 1 << 1,
	PRIM_RX		= 1 << 0 
};

enum{Bank1RegsSize = 14*4 };
const uint8_t Bank1Regs[Bank1RegsSize]=
{
// MSB first
	0x40, 0x4B, 0x01, 0xE2, //0
	0xC0, 0x4B, 0x00, 0x00,	//1
	0xD0, 0xFC, 0x8C, 0x02,	//2
	0x99, 0x00, 0x39, 0x41,	//3
	0xD9, 0x9E, 0x86, 0x0B,	//4
	0x24, 0x06, 0x7F, 0xA6,	//5
	0x00, 0x00, 0x00, 0x00,	//6
	0x00, 0x00, 0x00, 0x00,	//7
	0x00, 0x00, 0x00, 0x00,	//8
//LSB first
	0x00, 0x00, 0x00, 0x00,	//9
	0x00, 0x00, 0x00, 0x00,	//10
	0x00, 0x00, 0x00, 0x00,	//11
	0x00, 0x12, 0x73, 0x00,	//12
	0x36, 0xB4, 0x80, 0x00	//13
};


enum{Reg14Size = 11 };
const uint8_t Bank1_Reg14[Reg14Size]=
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
	RxPipeNumber	= 7 << 1,
	TxFull			= 1
};

class Rfm70Defaults
{
	static const AutoRetransmissionDelayValues AutoRetransmissionDelay = Wait2000us;
	static const uint8_t AutoRetransmissionCount = 15; //0..15

public:
	static const AddressWidthValues AddressWidth = AW5Bytes;
	static const uint8_t RfChannel 			= 12; // 0..83
	static const uint8_t RfSetup 			= DataRate2Mbps | OutputPower5dBm | LnaHighGain;
	static const uint8_t Pipe0RxAddress[AddressWidth + 2];

public:
	static const uint8_t RetrySetrup = Wait2000us | (AutoRetransmissionCount << 4);

};

const uint8_t Rfm70Defaults::Pipe0RxAddress[AddressWidth + 2] = {0x12, 0x34, 0x56, 0x78, 0x9A};

template<class Spi, class SlaveSelectPin, class EnablePin, class IrqPin>
class Rfm70
{
	static uint8_t _status;
private:

	static void WriteReg(uint8_t reg, uint8_t value)
	{
		SlaveSelectPin::Clear();
		_status = Spi::ReadWrite(reg);
		Spi::ReadWrite(value);
		SlaveSelectPin::Set();
	}

	static uint8_t ReadReg(uint8_t reg)                               
	{                                                           
		SlaveSelectPin::Clear();
		_status = Spi::ReadWrite(reg);
		uint8_t value = Spi::ReadWrite(0);
		SlaveSelectPin::Set();
		return value;
	}

	static void ReadBuffer(uint8_t command, uint8_t *buffer, uint8_t length)
	{
		SlaveSelectPin::Clear();
		_status = Spi::ReadWrite(command);

		for(uint8_t *end = buffer + length; buffer != end; ++buffer)
			*buffer = Spi::ReadWrite(0);

		SlaveSelectPin::Set();
	}

	static void WriteBuffer(uint8_t command, const uint8_t *buffer, uint8_t length)
	{
		SlaveSelectPin::Clear();
		_status = Spi::ReadWrite(command);

		for(const uint8_t *end = buffer + length; buffer != end; ++buffer)
			Spi::ReadWrite(*buffer);

		SlaveSelectPin::Set();
	}

	static void InitBank1Regs()
	{
		SwitchBank(1);
		WriteBuffer(WRITE_REG | 14, Bank1_Reg14, Reg14Size);

		for(unsigned i=0; i < Bank1RegsSize; i+=4)
			WriteBuffer(WRITE_REG | i>>2, Bank1Regs + i, 4);
	}

	static void SwitchBank(bool bank)
	{
		bool tmp = (ReadReg(READ_REG | 7) & 0x80) != 0;
		if(tmp != bank)		
		{
			WriteReg(ACTIVATE, 0x53);
		}
	}

public:

	template<class Formater>
	static void DumpRegs()
	{
		Formater formater;
		SwitchBank(0);
		formater << "Bank0:\r\n";

		for(unsigned i = 0; i<32; i++)
		{
			if(i==0x0A || i==0x0B || i==0x10)
			{
				unsigned long val;
				ReadBuffer(READ_REG | i, (uint8_t*)(&val), 4);
				formater << i << ":\t" << val << "\r\n";
			}
			else
			{
				unsigned val = ReadReg(READ_REG | i);
				formater << i << ":\t" << val << "\r\n";
			}
		}
	}

	static void Init(uint8_t config)
	{
		SlaveSelectPin::Set();
		EnablePin::Set();
		SlaveSelectPin::SetDirWrite();
		EnablePin::SetDirWrite();
		InitBank1Regs();
		SwitchBank(0);
	}

	static void SwitchToRxMode()
	{
		WriteReg(FLUSH_RX, 0);
		uint8_t value = ReadReg(STATUS);
		WriteReg(WRITE_REG | STATUS, value);

		EnablePin::Clear();
		value = ReadReg(CONFIG);
	  	WriteReg(WRITE_REG | CONFIG, value | PRIM_RX); 
		EnablePin::Set();
	}

	static void SwitchToTxMode()
	{
		WriteReg(FLUSH_TX, 0);//flush Tx
		EnablePin::Clear();
		uint8_t value = ReadReg(CONFIG);
	  	WriteReg(WRITE_REG | CONFIG, value & 0xfe);
		EnablePin::Set();
	}


	static void RfChannel(uint8_t channel)
	{
		WriteReg(WRITE_REG | RF_CH, channel);
	}

	static void RfSetUp(RfSetupValues rfSetup)
	{
		WriteReg(WRITE_REG | RF_SETUP, rfSetup);
	}


	template<int PipeNumber>
	class RxPipe
	{
		
	};

};

template<class Spi, class SlaveSelectPin, class EnablePin, class IrqPin>
uint8_t Rfm70<Spi, SlaveSelectPin, EnablePin, IrqPin>::_status;
